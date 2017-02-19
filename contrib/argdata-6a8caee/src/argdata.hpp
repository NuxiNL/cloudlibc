// Copyright (c) 2017 Maurice Bos <m-ou.se@m-ou.se>.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

#ifndef ARGDATA_HPP
#define ARGDATA_HPP

#include <time.h>

#include <cstdint>
#include <iterator>
#include <memory>
#include <vector>

#include "argdata.h"

#include <mstd/optional.hpp>
#include <mstd/range.hpp>
#include <mstd/string_view.hpp>

class argdata_t {

public:
	argdata_t() = delete;
	argdata_t(argdata_t const &) = delete;
	argdata_t &operator=(argdata_t const &) = delete;

#undef argdata_struct_name

	void operator delete (void *p) {
		argdata_free(static_cast<argdata_t *>(p));
	}

	static std::unique_ptr<argdata_t> create_encoded(mstd::range<unsigned char> r) {
		return std::unique_ptr<argdata_t>(argdata_create_buffer(r.data(), r.size()));
	}
	static std::unique_ptr<argdata_t> create_binary(mstd::range<unsigned char> r) {
		return std::unique_ptr<argdata_t>(argdata_create_binary(r.data(), r.size()));
	}
	static std::unique_ptr<argdata_t> create_fd(int v) {
		return std::unique_ptr<argdata_t>(argdata_create_fd(v));
	}
	static std::unique_ptr<argdata_t> create_float(double v) {
		return std::unique_ptr<argdata_t>(argdata_create_float(v));
	}
	static std::unique_ptr<argdata_t> create_int(std::uintmax_t v) {
		return std::unique_ptr<argdata_t>(argdata_create_int(v));
	}
	static std::unique_ptr<argdata_t> create_int(std::intmax_t v) {
		return std::unique_ptr<argdata_t>(argdata_create_int(v));
	}
	static std::unique_ptr<argdata_t> create_int(int v) {
		return std::unique_ptr<argdata_t>(argdata_create_int(v));
	}
	static std::unique_ptr<argdata_t> create_str(mstd::string_view v) {
		return std::unique_ptr<argdata_t>(argdata_create_str(v.data(), v.size()));
	}
	static std::unique_ptr<argdata_t> create_timestamp(timespec const & v) {
		return std::unique_ptr<argdata_t>(argdata_create_timestamp(&v));
	}

	static std::unique_ptr<argdata_t> create_map(
		mstd::range<argdata_t const *const> keys,
		mstd::range<argdata_t const *const> values
	) {
		return std::unique_ptr<argdata_t>(argdata_create_map(
			keys.data(),
			values.data(),
			keys.size() < values.size() ? keys.size() : values.size()
		));
	}

	static std::unique_ptr<argdata_t> create_seq(mstd::range<argdata_t const *const> values) {
		return std::unique_ptr<argdata_t>(argdata_create_seq(values.data(), values.size()));
	}

	static constexpr argdata_t const *false_() { return &argdata_false; }
	static constexpr argdata_t const *true_ () { return &argdata_true ; }
	static constexpr argdata_t const *null  () { return &argdata_null ; }

	static constexpr argdata_t const *bool_(bool v) { return v ? true_() : false_(); }

	mstd::optional<mstd::range<unsigned char const>> get_binary() const {
		void const *data;
		size_t size;
		if (argdata_get_binary(this, &data, &size)) return {};
		return mstd::range<unsigned char const>{static_cast<unsigned char const *>(data), size};
	}
	mstd::optional<bool> get_bool() const {
		bool r;
		if (argdata_get_bool(this, &r)) return {};
		return r;
	}
	mstd::optional<int> get_fd() const {
		int r;
		if (argdata_get_fd(this, &r)) return {};
		return r;
	}
	mstd::optional<double> get_float() const {
		double r;
		if (argdata_get_float(this, &r)) return {};
		return r;
	}
	mstd::optional<std::intmax_t> get_int() const {
		std::intmax_t r;
		if (argdata_get_int(this, &r)) return {};
		return r;
	}
	mstd::optional<std::uintmax_t> get_uint() const {
		std::uintmax_t r;
		if (argdata_get_int(this, &r)) return {};
		return r;
	}
	mstd::optional<mstd::string_view> get_str() const {
		char const *data;
		size_t size;
		if (argdata_get_str(this, &data, &size)) return {};
		return mstd::string_view(data, size);
	}
	mstd::optional<timespec> get_timestamp() const {
		timespec r;
		if (argdata_get_timestamp(this, &r)) return {};
		return r;
	}

	// Same as above, but return a default value (empty/zero/etc.) instead of nullopt.
	mstd::range<unsigned char const> as_binary   () const { return get_binary   ().value_or(            nullptr); }
	bool                             as_bool     () const { return get_bool     ().value_or(              false); }
	int                              as_fd       () const { return get_fd       ().value_or(                 -1); }
	double                           as_float    () const { return get_float    ().value_or(                0.0); }
	std::intmax_t                    as_int      () const { return get_int      ().value_or(                  0); }
	std::uintmax_t                   as_uint     () const { return get_uint     ().value_or(                  0); }
	mstd::string_view                as_str      () const { return get_str      ().value_or(mstd::string_view{}); }
	timespec                         as_timestamp() const { return get_timestamp().value_or(         timespec{}); }

	class map;
	class seq;

	class map_iterator {
	public:
		using value_type = std::pair<argdata_t const *, argdata_t const *>;
		using pointer = value_type const *;
		using reference = value_type const &;
		using iterator_category = std::forward_iterator_tag;
	private:
		value_type value_ = {nullptr, nullptr};
		argdata_map_iterator_t it_;
		friend map;
	public:
		map_iterator() {}
		map_iterator(map_iterator const &other) { *this = other; }
		map_iterator &operator=(map_iterator const &other) {
			it_ = other.it_;
			value_ = other.value_;
			if (
				(char *)value_.first > (char *)&other.it_ &&
				(char *)value_.first < (char *)&other.it_ + sizeof(it_)
			) value_.first = (argdata_t *)((char *)value_.first - (char *)&other.it_ + (char *)&it_);
			if (
				(char *)value_.second > (char *)&other.it_ &&
				(char *)value_.second < (char *)&other.it_ + sizeof(it_)
			) value_.second = (argdata_t *)((char *)value_.second - (char *)&other.it_ + (char *)&it_);
			return *this;
		}
		reference operator*() const { return value_; }
		pointer operator->() const { return &value_; }
		map_iterator &operator++() {
			if (!argdata_map_next(&it_, &value_.first, &value_.second)) {
				value_ = {nullptr, nullptr};
			}
			return *this;
		}
		map_iterator operator++(int) {
			map_iterator copy = *this;
			++*this;
			return copy;
		}
		int error() const {
			return it_.error;
		}
		friend bool operator==(map_iterator const &a, map_iterator const &b) {
			return a.value_ == b.value_;
		}
		friend bool operator!=(map_iterator const &a, map_iterator const &b) {
			return !(a == b);
		}
	};

	class seq_iterator {
	public:
		using value_type = argdata_t const *;
		using pointer = value_type const *;
		using reference = value_type const &;
		using iterator_category = std::forward_iterator_tag;
	private:
		value_type value_ = nullptr;
		argdata_seq_iterator_t it_;
		friend seq;
	public:
		seq_iterator() {}
		seq_iterator(map_iterator const &other) { *this = other; }
		seq_iterator &operator=(seq_iterator const &other) {
			it_ = other.it_;
			value_ = other.value_;
			if (
				(char *)value_ > (char *)&other.it_ &&
				(char *)value_ < (char *)&other.it_ + sizeof(it_)
			) value_ = (argdata_t *)((char *)value_ - (char *)&other.it_ + (char *)&it_);
			return *this;
		}
		reference operator*() const { return value_; }
		pointer operator->() const { return &value_; }
		seq_iterator &operator++() {
			if (!argdata_seq_next(&it_, &value_)) {
				value_ = nullptr;
			}
			return *this;
		}
		seq_iterator operator++(int) {
			seq_iterator copy = *this;
			++*this;
			return copy;
		}
		int error() const {
			return it_.error;
		}
		friend bool operator==(seq_iterator const &a, seq_iterator const &b) {
			return a.value_ == b.value_;
		}
		friend bool operator!=(seq_iterator const &a, seq_iterator const &b) {
			return !(a == b);
		}
	};

	class map {
	private:
		argdata_map_iterator_t start_it_;
		friend argdata_t;
	public:
		map_iterator before_begin() const {
			map_iterator i;
			i.it_ = start_it_;
			return i;
		}
		map_iterator begin() const {
			map_iterator i;
			i.it_ = start_it_;
			++i;
			return i;
		}
		map_iterator end() const { return {}; }
	};

	class seq {
	private:
		argdata_seq_iterator_t start_it_;
		friend argdata_t;
	public:
		seq_iterator before_begin() const {
			seq_iterator i;
			i.it_ = start_it_;
			return i;
		}
		seq_iterator begin() const {
			seq_iterator i;
			i.it_ = start_it_;
			++i;
			return i;
		}
		seq_iterator end() const { return {}; }
	};

	mstd::optional<map> get_map() const {
		map r;
		if (argdata_map_iterate(this, &r.start_it_)) return {};
		return r;
	}
	mstd::optional<seq> get_seq() const {
		seq r;
		if (argdata_seq_iterate(this, &r.start_it_)) return {};
		return r;
	}

	map as_map() const {
		map r;
		argdata_map_iterate(this, &r.start_it_);
		return r;
	}
	seq as_seq() const {
		seq r;
		argdata_seq_iterate(this, &r.start_it_);
		return r;
	}

	size_t encoded_size(size_t *n_fds = nullptr) const {
		size_t r;
		argdata_get_buffer_length(this, &r, n_fds);
		return r;
	}

	void encode(std::vector<unsigned char> &buffer) const {
		buffer.resize(encoded_size());
		argdata_get_buffer(this, buffer.data(), nullptr);
	}

	void encode(std::vector<unsigned char> &buffer, std::vector<int> &fds) const {
		size_t n_fds;
		buffer.resize(encoded_size(&n_fds));
		fds.resize(n_fds);
		n_fds = argdata_get_buffer(this, buffer.data(), fds.data());
		fds.resize(n_fds);
	}

	std::vector<unsigned char> encode(std::vector<int> *fds = nullptr) const {
		std::vector<unsigned char> buffer;
		if (fds) encode(buffer, *fds);
		else encode(buffer);
		return buffer;
	}

};

#endif
