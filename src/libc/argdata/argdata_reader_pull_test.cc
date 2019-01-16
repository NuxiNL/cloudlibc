// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>
#include <sys/stat.h>

#include <argdata.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

// TODO(ed): Add more tests!

TEST(argdata_reader_pull, eagain) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, fcntl(fds[0], F_SETFL, fcntl(fds[0], F_GETFL, 0) | O_NONBLOCK));

  // Non-blocking read on pipe with no data.
  argdata_reader_t *ar = argdata_reader_create(16, 16);
  ASSERT_NE(NULL, ar);
  ASSERT_EQ(EAGAIN, argdata_reader_pull(ar, fds[0]));
  argdata_reader_free(ar);

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(argdata_reader_pull, ebadf) {
  // Invalid file descriptor.
  argdata_reader_t *ar = argdata_reader_create(16, 16);
  ASSERT_NE(NULL, ar);
  ASSERT_EQ(EBADF, argdata_reader_pull(ar, -123));
  argdata_reader_free(ar);
}

TEST(argdata_reader_pull, emsgsize_data) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(12, write(fds[1], "\x00\x00\x00\x00\x00\x01\xe2\xb5", 12));

  // Message body too large.
  argdata_reader_t *ar = argdata_reader_create(123572, 16);
  ASSERT_NE(NULL, ar);
  ASSERT_EQ(EMSGSIZE, argdata_reader_pull(ar, fds[0]));
  argdata_reader_free(ar);

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

#if 0  // TODO(ed): Reimplement!
TEST(argdata_reader_pull, emsgsize_fds) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(12, write(fds[1],
                      "\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x0a\xc0", 12));

  // Too many file descriptors.
  argdata_reader_t *ar = argdata_reader_create(16, 2751);
  ASSERT_NE(NULL, ar);
  ASSERT_EQ(EMSGSIZE, argdata_reader_pull(ar, fds[0]));
  argdata_reader_free(ar);

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}
#endif

TEST(argdata_reader_pull, pipe) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));

  // Write a small number of messages into a pipe.
  argdata_writer_t *aw = argdata_writer_create();
  ASSERT_NE(NULL, aw);
  argdata_writer_set(aw, &argdata_true);
  ASSERT_EQ(0, argdata_writer_push(aw, fds[1]));
  argdata_writer_set(aw, &argdata_false);
  ASSERT_EQ(0, argdata_writer_push(aw, fds[1]));
  argdata_writer_free(aw);
  ASSERT_EQ(0, close(fds[1]));

  // Extract messages in serialized form.
  argdata_reader_t *ar = argdata_reader_create(2, 0);
  ASSERT_NE(NULL, ar);
  ASSERT_EQ(0, argdata_reader_pull(ar, fds[0]));
  const argdata_t *ad = argdata_reader_get(ar);
  ASSERT_NE(NULL, ad);
  bool value;
  ASSERT_EQ(0, argdata_get_bool(ad, &value));
  ASSERT_TRUE(value);
  ASSERT_EQ(0, argdata_reader_pull(ar, fds[0]));
  ad = argdata_reader_get(ar);
  ASSERT_NE(NULL, ad);
  ASSERT_EQ(0, argdata_get_bool(ad, &value));
  ASSERT_FALSE(value);
  ASSERT_EQ(0, argdata_reader_pull(ar, fds[0]));
  ASSERT_EQ(NULL, argdata_reader_get(ar));
  argdata_reader_free(ar);
  ASSERT_EQ(0, close(fds[0]));
}

TEST(argdata_reader_pull, socket_fd_passing) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

  {
    argdata_writer_t *aw = argdata_writer_create();

    // Message 1: two file descriptors, both placed in a list twice.
    argdata_t *fd_directory = argdata_create_fd(fd_tmp);
    argdata_t *fd_socket = argdata_create_fd(fds[1]);
    const argdata_t *fields[] = {
        fd_directory,
        fd_socket,
        fd_directory,
        fd_socket,
    };
    argdata_t *seq =
        argdata_create_seq(fields, sizeof(fields) / sizeof(fields[0]));
    argdata_writer_set(aw, seq);
    ASSERT_EQ(0, argdata_writer_push(aw, fds[1]));
    argdata_free(fd_directory);
    argdata_free(seq);

    // Message 2: just a single file descriptor.
    argdata_writer_set(aw, fd_socket);
    ASSERT_EQ(0, argdata_writer_push(aw, fds[1]));
    argdata_free(fd_socket);

    argdata_writer_free(aw);
    ASSERT_EQ(0, close(fds[1]));
  }

  argdata_reader_t *ar = argdata_reader_create(100, 2);
  int fd_directory;
  {
    // Receive message 1 through the UNIX socket.
    ASSERT_EQ(0, argdata_reader_pull(ar, fds[0]));
    argdata_seq_iterator_t it;
    argdata_seq_iterate(argdata_reader_get(ar), &it);

    // First file descriptor should be a directory. Release it.
    const argdata_t *fd_object;
    ASSERT_TRUE(argdata_seq_get(&it, &fd_object));
    ASSERT_EQ(0, argdata_get_fd(fd_object, &fd_directory));
    struct stat sb;
    ASSERT_EQ(0, fstat(fd_directory, &sb));
    ASSERT_TRUE(S_ISDIR(sb.st_mode));
    argdata_reader_release_fd(ar, fd_directory);

    // Second file descriptor should be a socket.
    argdata_seq_next(&it);
    int fd_socket;
    ASSERT_TRUE(argdata_seq_get(&it, &fd_object));
    ASSERT_EQ(0, argdata_get_fd(fd_object, &fd_socket));
    ASSERT_EQ(0, fstat(fd_socket, &sb));
    ASSERT_TRUE(S_ISSOCK(sb.st_mode));

    // Third file descriptor was a directory, but has been released.
    argdata_seq_next(&it);
    ASSERT_TRUE(argdata_seq_get(&it, &fd_object));
    int fd_bad;
    ASSERT_EQ(EBADF, argdata_get_fd(fd_object, &fd_bad));

    // Fourth file descriptor should still be a socket.
    argdata_seq_next(&it);
    ASSERT_TRUE(argdata_seq_get(&it, &fd_object));
    ASSERT_EQ(0, argdata_get_fd(fd_object, &fd_socket));
    ASSERT_EQ(0, fstat(fd_socket, &sb));
    ASSERT_TRUE(S_ISSOCK(sb.st_mode));

    // End of sequence.
    argdata_seq_next(&it);
    ASSERT_FALSE(argdata_seq_get(&it, &fd_object));
    ASSERT_EQ(ARGDATA_ITERATOR_END, it.index);

    // Receive message 2 through the UNIX socket.
    ASSERT_EQ(0, argdata_reader_pull(ar, fds[0]));
    ASSERT_EQ(0, argdata_get_fd(argdata_reader_get(ar), &fd_socket));
    ASSERT_EQ(0, fstat(fd_socket, &sb));
    ASSERT_TRUE(S_ISSOCK(sb.st_mode));
  }

  // Reading the next message should cause any unreleased file
  // descriptors to be closed. This includes fds[1], meaning the next
  // read on the socket should return end-of-file.
  ASSERT_EQ(0, argdata_reader_pull(ar, fds[0]));
  ASSERT_EQ(NULL, argdata_reader_get(ar));
  argdata_reader_free(ar);
  ASSERT_EQ(0, close(fds[0]));

  // Directory should have remained valid, as it has been released.
  struct stat sb;
  ASSERT_EQ(0, fstat(fd_directory, &sb));
  ASSERT_TRUE(S_ISDIR(sb.st_mode));
  ASSERT_EQ(0, close(fd_directory));
}
