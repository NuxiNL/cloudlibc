// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(crypt_r, bad) {
  // No support for Blowfish.
  struct crypt_data data;
  data.initialized = 0;
  ASSERT_EQ(NULL, crypt_r("Some key", "$2y$14$wHhBmAgOMZEld9iJtV./aq", &data));
  ASSERT_EQ(ENOSYS, errno);

  // No support for Triple-DES either.
  data.initialized = 0;
  ASSERT_EQ(NULL, crypt_r("Some key", "wQ", &data));
  ASSERT_EQ(ENOSYS, errno);
}

TEST(crypt_r, examples) {
#define TEST_CRYPT(key, salt, hash)                          \
  do {                                                       \
    struct crypt_data data;                                  \
    data.initialized = 0;                                    \
    ASSERT_STREQ(salt hash, crypt_r(key, salt, &data));      \
    data.initialized = 0;                                    \
    ASSERT_STREQ(salt hash, crypt_r(key, salt hash, &data)); \
  } while (0)
  // MD5.
  TEST_CRYPT("", "$1$hjGFphfY", "$CQmTgzRaYig9u2QeknW9G1");
  TEST_CRYPT("6", "$1$.RacOpcf", "$0pJU.zRlztr3Tf2EsAxFe/");
  TEST_CRYPT("NA", "$1$9StLKoEB", "$xXuCz8SPzujmak9q8SeXI0");
  TEST_CRYPT("XEj", "$1$1tPQ00mz", "$XwGxiTd8.ApEZaXSEMaSu1");
  TEST_CRYPT("Jpck", "$1$1VL/6ulZ", "$O1lLACVWaB8ANgqbgsLcf.");
  TEST_CRYPT("YrJIE", "$1$LX//s3FB", "$Zhbm51NwDAHKoDtqkvwNL1");
  TEST_CRYPT("Tq4dkE", "$1$V4kDLu9n", "$ydtXAjblk063SGjt81kAi0");
  TEST_CRYPT("jy9ZDST", "$1$Dg1lh2gQ", "$yUEo0iJn.53f3nLPnHMP71");
  TEST_CRYPT("UguX2uaM", "$1$61sOf5xl", "$q2n/idp0RwWVybF4BVXxy1");
  TEST_CRYPT("7o3mTA8S1", "$1$eR1v2PP0", "$cyEwFaVAqM8p/hvkb.yOH/");
  TEST_CRYPT("wtzDv2wjYU", "$1$d7NGoDFv", "$TfWx8WXNvWDSlWGmzoLNO0");
  TEST_CRYPT("oQY6omQUFmW", "$1$WaQmE48I", "$sLBclqeVW2ig4ZPIG6tyo0");
  TEST_CRYPT("ZOaosV1Ezjs3", "$1$0ZmWPdxD", "$0V.uW95EuuWWmQV08xASO.");
  TEST_CRYPT("ALOSMSRsXrUh1", "$1$Svlj4V1E", "$aTqbd/vFOXGfe0KZE/yan/");
  TEST_CRYPT("agt3dPQ4qJn7EF", "$1$5jU591nG", "$pUvMnFn6L5PXEePnStoz51");
  TEST_CRYPT("oyQNvK4nDhzjwM8", "$1$Rsrbmh9q", "$AaqfwjCXxtnfor3kUcAY11");
  TEST_CRYPT("ZzqKaUuvWgayNJKx", "$1$ZHjwSRiO", "$/IeNMZMG5orQAp9LqYc3W.");
  TEST_CRYPT("r0NhfVgTAvYZ5rkYw", "$1$txYWe9oP", "$48QVa3reByKuA.BkIVRFy/");
  TEST_CRYPT("oxtC0qnsiVzTFObRd9", "$1$HGogfqpE", "$tpkZ0lr6EWhHyXUuxcCAz0");
  TEST_CRYPT("nPedkEzcmmyrCJnstaZ", "$1$ZfpzIqyY", "$oAVkLGH3Jyxp7rtdXM3Rg/");
  TEST_CRYPT("sZG1lhZCAak3YxXibkkx", "$1$y4EyBc4v", "$qUo3CiQoOEFcYS3.zuxag.");
  TEST_CRYPT("BZ371f7iKF9k56TQfVvWV", "$1$mufJd/6D", "$FQTP6Hx6XABuZv98C4imh1");
  TEST_CRYPT("MrDXGXIFudDkb6y6ASdiCY", "$1$EY3cyG3P",
             "$0yEKPQeitJlXvFCtIhylK0");
  TEST_CRYPT("Y0cMEh0w8Nb63DHXzaZSklm", "$1$W32h4qjD",
             "$bZStsd.s.PG5.DFF7Qpy01");
  TEST_CRYPT("I0gzFF2J4SVCLfC820H7Bg2a", "$1$S59uEvga",
             "$JV8Qa9fr1fhO9WxG0is1A.");
  TEST_CRYPT("dR1oUORojO3WQrYlemQ3KbQr2", "$1$h3/lE6Nd",
             "$bzkUoQpECEXtw8r/fVE/e0");
  TEST_CRYPT("zVhkuF85dwYRzltLnUj41lrMWD", "$1$N5VFzbjs",
             "$X4y1Svwxm0tfsVWg72TQ/0");
  TEST_CRYPT("ZXyXjkSG3VQP3oYvpajKwjXtaco", "$1$r4Csw2xc",
             "$yXkR1ZHOPQLQXrg/g9cX81");
  TEST_CRYPT("qNEcNa43RrjIRz6tZDwHzGxziuu6", "$1$c/rNG3Lk",
             "$i1Jaoq.6YY5lc6QlJib7w1");
  TEST_CRYPT("BqpKX4a4yUmIzI3qyNWPN19kb5WEa", "$1$jH53dgjW",
             "$c5oLSc1NAXl4l4HF/oG/q1");
  TEST_CRYPT("5XE1FWSHBwQ7GNkZNAuEWo8vuL0PrN", "$1$xc7MZFvA",
             "$nGLTWlrs8grwEe84A7O1P1");
  TEST_CRYPT("uWYwqv65kHUcfkxOXSQrcKcvrHUk4Rt", "$1$u4J2s57v",
             "$aeQfezUNXJsyo4aEw1IfK.");
  TEST_CRYPT("wtAtTbKHQeFRdiw3HA034crpE6VLvoWR", "$1$1jKTDaOj",
             "$M5efsThrlSKrj6N99Xc0D.");
  TEST_CRYPT("z0Pqh7lNRCQSPvoa9iVTljGkA7QM0YsQm", "$1$7gzsxxwW",
             "$uDyHTD4Q9bzXmNrVIs6sU/");
  TEST_CRYPT("9iRQ81jltHydbNFQlkRWXor8eh4CGT5g98", "$1$iR13.KFp",
             "$.25Vu.qHWcgYR.KxrHWgS1");
  TEST_CRYPT("HRcMirCX63y9K7CXVFaXcUaSeDqcW0E5ktY", "$1$Mz3arym9",
             "$NPyZfMf9isSXujl5WCd9T/");
  TEST_CRYPT("zbfvOrf5PfpRfExr1170Tca1HspamPIdnPHb", "$1$Qujjgtzj",
             "$CFJEUSGObuono1HbLjGzN1");
  TEST_CRYPT("ymdjfkZUEqPxaa643YMlN8Qfz8cBjtPmS6z9p", "$1$eS8hjSdi",
             "$SFlECzt9n1nzRsg917Aic1");
  TEST_CRYPT("3jAq1rQOgykuNEawd5cDvxfwP3MITxHis2xBYY", "$1$IN4E8Wey",
             "$2H85g6CW/almDaPCMR47I/");
  TEST_CRYPT("iuOokoL681mpOX8bIL9GZqRrMTlEgxIFAbVJzM8", "$1$jyJxniP1",
             "$bh2GYxj.lMLW0lxBiyCgk0");
  TEST_CRYPT("CqyOZE2hbWo6cAMtSQR2XqNKfEolYRYnDEnGEksA", "$1$Zd3rygIv",
             "$GLHAFaDWRy7dMgj13iewN0");
  TEST_CRYPT("dn6KV2I9BWpQcqAD6r175vmCHc2N4jLA45cktUyfC", "$1$bh/1AL95",
             "$ZsXVSqv4h13d0Oq8bkmYD/");
  TEST_CRYPT("HT2ZdkbwFM7Bf4m79crTPZs2IpFQWl42cEuUGPIL2B", "$1$WQL8KqOc",
             "$lPLL8tNVR8w1z8R9n54XI.");
  TEST_CRYPT("jqhJDF7imCltZoxoIkx7GRLYoAWl9KefawIDaBT3WOx", "$1$gX8rHSJ2",
             "$GG4ZB3h7VQyIsbZ4N.clG1");
  TEST_CRYPT("g7UzGm6FuV7rnmFeNewi38kMx8Uw4lVPAkKsBVBIyfZu", "$1$aXSH7wJS",
             "$PaJK3gHLsPJ2cUQpXIIuZ.");
  TEST_CRYPT("4MEmF9rERlZBXFNl17cgTWphJ50gvTqt0YyqRwYqmHXuU", "$1$3Mj7JIsT",
             "$fD5ZLcMNC1/.Vl6GhqEJQ1");
  TEST_CRYPT("9hYZM0bH2BmK2GKApmgrO614CZ9L23qFba4QeybmDuXqnu", "$1$9BMO9TRk",
             "$KSzw4S6KJVmTxiSIqpZaX1");
  TEST_CRYPT("Gtdi6rPh5MM2NOlHIUmWOs6y90c1cxSBLnWobqoHVPzCv1Z", "$1$/obxZuOm",
             "$3.1zvq9HVzlx1jT0kqJVR0");
  TEST_CRYPT("knEwxtDwFCkwuPn965cwLOhremXjWH5t6MhL1osFjoHv1J2h", "$1$HRHmtnm3",
             "$RTrGIddLHW6vmpEHZygoa.");
  TEST_CRYPT("KAznmlXX7sA0XaGQMfnHJ9AbNIgvq6hZArQv0WDi56dJ6SeiR", "$1$6UqEHSTF",
             "$5N00OV/Y1W73Sly.w47S//");
  TEST_CRYPT("LRmn13HOvf4ZlUO2cDOMijKtjiCh9ibeF7Gmz6kR0jnnDgqkeq",
             "$1$ub5AFLUA", "$Kir2j0v3m5vIRZzAREmnf0");
  TEST_CRYPT("zGM2dhEc5F9pvbrzJ3Cd2RuD1whfcWNxBgJMjJ5CmKcqFvuVOf4",
             "$1$RNJ6.wnI", "$8AEEx2MCZX0V9RqDHfEFc/");
  TEST_CRYPT("RUCUThVKzn3QH93AMxsIbTkDWFuEIPmix7AKfJ9xe3heU7FcJZhm",
             "$1$4F1AB3rz", "$7dyc5WVpDTspk7Q9gQC6R0");
  TEST_CRYPT("XVpNzHHt2MDIeueGLMi0xK5tzgVbFYHDGOFnaUGACjrqIdNTlns3A",
             "$1$615cQ0XE", "$T5t8vr0OtiZ.t/fjXcss7.");
  TEST_CRYPT("jMmpXJFvcUcbg3ACxDFSBxxoLK7YHo8KR8m9Gvp6kKVh45gm10eRFO",
             "$1$IamaA/H/", "$KcW77b7nWaH0A1.etc/JD.");
  TEST_CRYPT("SAy9JcW5WmZnI2Q4w9nsV9myP0MT6apGxrC0z86pDnP96ayPHJlrVBA",
             "$1$0zOeMcjo", "$16bz1aT26RpjtOC5vOSuc0");
  TEST_CRYPT("6ujtqL4kwy2c5Uurpo1K0vMN35yOqyNYTSnTJxyfe1hxRJWZEZKeW7xH",
             "$1$T5f6IE37", "$Nn2mhwEZ/wbGSeOZnc2hP.");
  TEST_CRYPT("JURmkyKmZkZkJfwZhcHTiXJaYf7Voh47PH1QZR48Ns0RZ1EACn85IsEag",
             "$1$dnt5SpJk", "$fpaIuPyuZBdvMZ8LhprSo0");
  TEST_CRYPT("YiZBx58Bl6kFSGfp4nWwlFhltdspW8LrIiCcTLdOb8QOTuFQ4vmL4WCDXF",
             "$1$FAYkAxXG", "$7TrFFV9A5GE06a1egtzD10");
  TEST_CRYPT("DmPc3DYyVVqCpw9g1Ffar4HjkXet2v6j5fcaJtF06yklNlyy4djuETGuny3",
             "$1$jCgIyl6L", "$m/YM.AdCeEAM8af1sHRBv0");
  TEST_CRYPT("HZJmX7vceTvVCPU37mejhZsUIX5DuLVA9NlLUn1MInwmDx7hIjYHjbTolU7t",
             "$1$/P.ocLw/", "$g8BaEx9XlOztBqo2kiwgM1");
  TEST_CRYPT("Csgcb8KJ91YugJ03rEhzvBAs3dQWaixPbLZTGzRiKfauFMwwdVjpW639IptKI",
             "$1$blJ38FLa", "$r0XzqNoQmmhHPr14eyhpc/");
  TEST_CRYPT("QBkR3JrdbCp08vfiKngv1eNALgmywMN32L6qCwPkw8AbTejjsyVBZvaegqKoTG",
             "$1$XTshCw1p", "$sB6dl1yd2FXMREedVBDx6/");
  TEST_CRYPT("504AIt7O26LvyGgkN8IfvbJbNyzMowJhRVijPqYSrSB1wOad9EdK9evpPDs3lbx",
             "$1$LyVD6qqX", "$1LpECVWBPIQLyErcbzgEV.");

  // SHA-256.
  TEST_CRYPT("", "$5$7XXVmMRB7rc1a..e",
             "$AKly1P8RCIYOr13pkVRXaSQjx4.SO4yMuP6nl1FWZI5");
  TEST_CRYPT("4", "$5$uXkzkUHyTNf/Ap.G",
             "$k5HBFOzj0xwpmxTlpAk50GqHSeBgZSmyx8MZvH1zusC");
  TEST_CRYPT("sL", "$5$oAiZXmTcV1Ge1DDv",
             "$0mh8GNEMsmMXlo5NT2IYZT/SVxnzU4.ZEipVHdwFNU8");
  TEST_CRYPT("BiL", "$5$j9jE2MFAEVZimpmQ",
             "$RN9QJEGpj7G4zm3gpRQl4HwBeinxEK1Hr0lu.giy8I2");
  TEST_CRYPT("SmBC", "$5$UQX.LmFzp1UukDd3",
             "$kLF7lDQmP6kN5sQOD616nnqzOzO3f1CRgCNhd5R0zbC");
  TEST_CRYPT("XmMEO", "$5$HGgfyS5iAetAs3Wu",
             "$9tAnLMc4nHqgZQ0uBULR2nfVfxT0kDoRuVEE3B1YEv0");
  TEST_CRYPT("hmsMhi", "$5$5itDnBzbGURT.8AM",
             "$NYAU0g42m08IzTPz3PP7Q3UWf8cBRkwBav6top/Oth9");
  TEST_CRYPT("84xJO7l", "$5$ckigpcYuuuuC2awG",
             "$F27SXgn.QhjzBouqFmE735BQ49kEB8Ik2u2NurLnKp5");
  TEST_CRYPT("tHllu0TG", "$5$Q9Nd8DTjXexzlgcc",
             "$Z4Jx2pa7pNG6zqkULm2hphTCy6kDks8ho6GteEBq6s2");
  TEST_CRYPT("cL8T9cneJ", "$5$wFWdhqVaf/34/dMW",
             "$PXOvK4nP3YaEaO87PR1YyNJuXmiOh/9UMDv2Q/qe8uD");
  TEST_CRYPT("cC9RFGEiQk", "$5$kEg7vVf3r4f1dZgy",
             "$4.EwI09H/aX0fd6S.gUKDSXsOWXp7zxSP7HBhSpYBY.");
  TEST_CRYPT("WfXwWOXFi2u", "$5$PH2ADJz.bATrJOcF",
             "$prma4LQxnUHwN/QpPdPo8/liog/hHPMNf98o.rVdPaD");
  TEST_CRYPT("6UsNMtrEpIEx", "$5$TZL8nZP0fUsn5XEY",
             "$LkhUDjar4N2XX9cXcIZPMDrPeahW/Qq7rimlKBuAqS6");
  TEST_CRYPT("J03RahsuwEfZz", "$5$oh8i1RQNWyyR27/v",
             "$Yp25DCqvivSkM9Odcz.VlgyXkHpEGRT6BRjv3D.I1T5");
  TEST_CRYPT("zJtnhXFZqSfWPu", "$5$SMEeHszAIICbL4xS",
             "$Fv7KBdRu.hvrvvkVVQ3OtaE28vmI5LP95ceVVrzB2E9");
  TEST_CRYPT("TaXGg2yEQfDCtiq", "$5$KinKEJfZzwnc/.TA",
             "$82hVQ4P44XSXJR5zD7SeZmQlt27hmh/0bvDN35FgkZ3");
  TEST_CRYPT("4xabjcsJW6QZC79f", "$5$hqZ1tlDP6fivvSpa",
             "$uSbwY8NjKJ0T.4WxBzgeuDLqk2G1/l.HgQ3gNGiZHg2");
  TEST_CRYPT("fn3RmY0APlFUoGNks", "$5$YF/T1GQ0UDOYFdVg",
             "$vdEgflbjypeLN8SZxhyf9F6.8uI9SoFp3YpoDZh0hbC");
  TEST_CRYPT("KLgnDsqRogFqszLjh9", "$5$ytQWWxKD3lsUN0p0",
             "$K7bxAlOeZ0tg7abFo2SxPhB1A4e8MKTD9.hWmXOLI71");
  TEST_CRYPT("jjjPb8eR6Q5MZp0YUkx", "$5$mCwngn7Tl.nKnQwV",
             "$GTNeTZBtrtPf/GCmFJPxr/4yQVOe/cLwOB2XqkYtvb6");
  TEST_CRYPT("lXNRmfKfWO57hBdYU1B9", "$5$GddOKKPnPuY8rttX",
             "$SICRyTaxFO4Z0gTkq8WjyYRVhy5epRLy3MklA/Mtj52");
  TEST_CRYPT("sqKwT0oL9rSXNebDhi0GW", "$5$5nY/hD1GRQbvzvGU",
             "$hW/HvSniXCcjVGrOj0Miw/EolPQrI1gcMWNu1Vh1JG2");
  TEST_CRYPT("kDKvobRPFmgZAWYUET5qJL", "$5$A/lu8ChkTVj7r1lJ",
             "$6Kjuwh4riWMPAQuTBT8ho92LZ3ruUYn4jBABZwDEbqB");
  TEST_CRYPT("sHi63UUEuLe5aXJExw3P0KH", "$5$xhN6Ef06w6p2C7Ps",
             "$uKuaufg0e.6QAUedspJPy50WbKdDJNf6vRVzYKUnQF4");
  TEST_CRYPT("X6DPUxuKmyBgSBjL88qRnpgc", "$5$yks2WaiOvMXlTr.r",
             "$U1i/MjCwsSjHUCpzseo6H8YrSDo6nyGbfr96YK5ibM1");
  TEST_CRYPT("ONw1aUW7hftGwWZxfrIavz6hM", "$5$gTZ.Hlo9qEyRpLyQ",
             "$GRMtigrWwfaElOYrLbee4UiSdMWq/7FgfmCVlWQ3Ti3");
  TEST_CRYPT("yLU3adiKxaJv7PZybLIWSjtfb4", "$5$SXCFmyz.fwvavcT3",
             "$6vpzhbW5PhtWVgDS1vbo79mMw75LLUEobgDhsvV9nkD");
  TEST_CRYPT("eVyjDHtdthLGmFVhtfNRfkDVgIO", "$5$3RH6wyJ/56MjTQKq",
             "$iP.wcSByzINhRdBJPwTeFtqtq9WTrg0B807Pa7m74/4");
  TEST_CRYPT("Pkpi6aguWGofhwnEbwTwbdKFtB3Q", "$5$C6opybtyqzmmNCar",
             "$R8f/m5RHPq50AhXE7zg3Qf2.KCZIpbKueuwFJq2N308");
  TEST_CRYPT("q0jHtQ3BevUHFcpW0Wo2YvcZ0Y3b9", "$5$oK0Zjey4msT0wIOc",
             "$AyMROoEnRSIdAFndUwzR2lc0H4IJMrocXQOXYMefVC6");
  TEST_CRYPT("wZu2bKeRfNt4khlDY9BEkpsrSdgRVn", "$5$AO6hVanepFshbaM3",
             "$0s2jlU/P3WKDkUDDBOHucOWEaEYZ2BDllLH5mfxSgtC");
  TEST_CRYPT("bFhiTRJzcWgLTFrfEUXErs3utey5jfD", "$5$vYQSryN8GAIsCOwX",
             "$wKXJhdVxPQbANq.D3emk.5Cusy6rdC2DZX3Lzu8JxH0");
  TEST_CRYPT("UcVUzljpygNnUpS7X47kohzCnhjDWUtA", "$5$kX0siFOIPRsQtQ7u",
             "$xH2R5B/wPGlf2v3KLiZvhZpkeI3LRxvyPC9Bt212oF7");
  TEST_CRYPT("0eFzi1BUih4qf6iIqcvj6EC3WqoXSHvmo", "$5$Gh/yGFga.PXVkuLK",
             "$vsytMr.i/a49pOONljvWmmcNRbUuSwBJndiU01yYxf/");
  TEST_CRYPT("bHsAMnXWZGhrc2KYZUNwbUldPHimjbSFFm", "$5$BXVe/T40hKkyovA3",
             "$WHBiebLIC5eQfMZfgjlnXBunuf7Y9tUM1OnNDMXiE3.");
  TEST_CRYPT("kctv2cgbjSWe6OC4P3ON5YwzIpsgohJcyov", "$5$kHwKYR7WD.J26531",
             "$izXJT2tQMWqRfBzgQ.D3NANi4wZVdRG/HU/J3lMGCV3");
  TEST_CRYPT("EbnuuJDYuotHIkwLaBhIdWEGux75XMsvFykZ", "$5$Y7kK1/khjz8PQXc8",
             "$UDRGDOE0h3rsjkgVKJfk1fFC7Ki16Tk4ZhUX7YyNel1");
  TEST_CRYPT("Z7FfyxYwD62PdQ9zhfQ2aX1PbBeDCQBimsBGP", "$5$TG9xTgyEgaigADVq",
             "$HaQu7wYwbU4iSNj48iQHFARmh9IjMECbsUOWZ.4j84B");
  TEST_CRYPT("BW5LaJZziTfQpAbxD3X1k6MleO3LlxJDshyg8V", "$5$vYRofCxmPVOKJRCz",
             "$g05SyHa6Ht4b0M3mvhmPmtcbFVlOa0Y5hKcXDODPR23");
  TEST_CRYPT("PeJk2zWBQMXBruIBEq6Whu2fbw2D7kWEcgzLULJ", "$5$JR5xWKpBF8lNkBB7",
             "$XbOmLEGxCynSsWJ5rZvZNkGAt.mKv77S1TUKIQ9YhO8");
  TEST_CRYPT("scU75ngT8nW1y5xlLYcX49gZjucffEcCEkLjSPTC", "$5$/pPHK6s1l6/DiW9G",
             "$EXNvZksWZXRQ.ZZu.eSe6MR37WaGMbwffMF7F4qP5y3");
  TEST_CRYPT("RdU8Fj46Dvyf68KkCmajYoZTbJe5hejTDxlxgzyyn", "$5$pdg6MNHBZBwtF9LJ",
             "$rvDC7I6yKx7TMTwMDyDlnPpY4PUg0TRSZRMGkDQLtE5");
  TEST_CRYPT("b9khaAiR3wHAuriS2L5RSa3G21IbNXJVjrchgIgek6",
             "$5$/dvg594k2P8BhYxN",
             "$cFXKiToIJO6JH3BUIWQnYPv95K9eybVv.nRLhNtUI3/");
  TEST_CRYPT("bni4m7wROBNkmmhW46YvtARmY8FMfcyMSoBtU9koiog",
             "$5$71xc05cxsQIQkdF0",
             "$TBtVN0vzKoT0ld5xee2e0UCK2VYFtOzPuuNdQ5LUbf/");
  TEST_CRYPT("MvuAf9EbMJxkFPsYtanUDnAJGmV3ozhnJr1awipjjQte",
             "$5$Xw41aQ7nPSP7BwLe",
             "$0eKkBVeuUud7O05y47YgHqYOP1T0AaBaWSncX6VvpCA");
  TEST_CRYPT("kIwzKg92jpb7l2Ao7zM0LC7h0ODC8otDkwryJearc50Hb",
             "$5$OTfNs2z3ACRCKR3W",
             "$JlCAshWJBI3hr/unJanZRMfcag0lDU5ChImt6EOR8w9");
  TEST_CRYPT("pNK629Momy834El5ucfGmbtmGbBTiGxbLjfSRboI0HFnpy",
             "$5$EbiHtKvdY3Ol8wZR",
             "$81HS7bpK7WtbnDezwv/qHfLPtcgPxxpxyebi/bSBC0A");
  TEST_CRYPT("ZgTkPYfwrVIRle7NwYcghQdNDNkBPHkbJx9wh1xaViwtNcU",
             "$5$0qSw4O0fLohr0cKB",
             "$DzKkGfCGHMgFO3NzY1x4Bth86anfCliUPXjzOncpCU1");
  TEST_CRYPT("9miZWm8wrcbpkipBAUwVhCLHxBMLRrUEKqtU8WTmdwIVqXpW",
             "$5$73sNXj.SAAueriOB",
             "$XAuRzT2mQK54OkV5LKEQU4Hc9JA1smPd5TnKACRr0s6");
  TEST_CRYPT("XcfkD34vEeZhz7DiKsWlgzIaiDincMcERjV5NRFcYCA8IGaTF",
             "$5$OpwAAqBOQ2R1sJTk",
             "$d2KKbMEmMJejR3.R9dQeu.kDSZkth8OuMWVK.EnDJb0");
  TEST_CRYPT("AxtTTpJ9J05Ete0yxZRd3OwYjj54dzlROpMGvyhW97CmQZlSU2",
             "$5$uwCjPCwdNuJINUwU",
             "$ufw03NvLjLqarRHn3wxmP61zAgAjC2PDbgvRMDoqSD4");
  TEST_CRYPT("TgdHOur2vqS338t3EYJgw2qns0zMcyM6jqKUdxfjVgshgIP9yBR",
             "$5$RramSKYbsDJ7lHW8",
             "$NVMQAVNBazFOoQdruECxZx6nO86d/nioz0pBky7Lmo4");
  TEST_CRYPT("nWTbBBN6zkHxPwclOIFuHy2vcj8FnqinpC96OGIMwSG74wOgbsx8",
             "$5$.iNEMf.y5yt/j472",
             "$er4mGqtv7gqaY89ilB8kka56ZGXAWhZY7ASPa48cGQA");
  TEST_CRYPT("JTSJvtn76eyMFRgPPyLv59PDD7zDdkmdGdqtPmhMPiH82Phb9qUds",
             "$5$a.blg0v02vW3xb1o",
             "$fM.ELXJZRiqybrZNF6BAu7Be88EXt8sr6AT0TVz9wG/");
  TEST_CRYPT("FpOrCjnPseBQKAh9y7obIm7pcRVsXB8muBXtE9cpc3Vei5QzGK66s5",
             "$5$IGuFN8dBq4KL4h/l",
             "$dlQBQYIb4fIPA70AgPToKcQqwbxH27Q3YaBRy.UF8y9");
  TEST_CRYPT("rBAYzm5IxfWyYqkDr0qq2TwMZvYpNswGjY5P4IjU9hjyQOELtdFg5iy",
             "$5$awWmDcGGPpJ6.Xh/",
             "$0u7Oszs9fGhftFoqgFvk0fY.VQcGutl7JlaA0RY0il5");
  TEST_CRYPT("WmSu0ikjovK2OdhdrafHDXDdDT5yp90azoAtZTl8YgIWMYetvm9UA2NY",
             "$5$fUcd6xbHWGDzB17b",
             "$WGVWHiC9fMzB5jfwlHerDbKxbnyPtsJBHSC7GptfBJC");
  TEST_CRYPT("JGHUAIXHj6hOfHoQHXKwXLe8xRqBeyYmAlvv5uVaSHaPemc11HoXz1DRu",
             "$5$9N.AsHLAj1a7yCgq",
             "$ocrp467S6VGDyFq68zBIoq27CE2X8PY8gmH.J.sZ3UC");
  TEST_CRYPT("Z7uyQjOmGu21d4QPcJehwNWxZkPgRSJTQYs5LbNRZ5zivGJIxgclMfrbXJ",
             "$5$2KSnnp6rgAfFnKUq",
             "$s4o9WBIHaDAKnRp7eXghFWUcMFYHGZfMdGPNJ9.cn08");
  TEST_CRYPT("P36AuX8bOFbjuxHaLtYXwX1UpNKEc5PSluMMT2hpbG43sDHhPhjTGxQzXAF",
             "$5$dY3dTvhk03OOHLRZ",
             "$FqWSDcviw.koS6gFUbZxwtuMTBqLVnUSEr.LWp3rQRD");
  TEST_CRYPT("VWHVibtkLRGUz8ioDjhcxdwfLKdDlqwWZ4faJmSj5lZf30XKgW7JMfXLHE0G",
             "$5$eUHxaUIR2wwpQZrr",
             "$Fww2e6/zoeDTWlPDBn6sXNpXePWmoNNFK9lyv.pQWa.");
  TEST_CRYPT("7qAIgs1wYmdk2K3yl3Lm6BgbBKFgNGWXCtDaAacp4dLaEskENtBgEJ0rrj07W",
             "$5$9o3vWQyO0F7qsJpb",
             "$PwyxJKzMSqH.VNzpPO.z1H34TGTHpTusF.FGi8chmv9");
  TEST_CRYPT("VmQTg9lNnH5tf1x7EV2ifYy9UdrcPmv1G2EPAGzw1nZUHiYzkuiF1JBWw2SjyM",
             "$5$L7eV4rToFSYadzOE",
             "$olmF21Q4JSZXMbLD8fCBQA3.4rvJpOfvc4HQbQJDyK8");
  TEST_CRYPT("XaAUfiRwiJ06ZtUFnewOazliJiJkaPIAp9vZQGGOFOHzNpJxPBuCR96AFBFUjxd",
             "$5$EWanGSU4Zl1T4qPF",
             "$./vwzSmLaOBiZ1/.E9nSsc/ikYRXfO4KUyP9G9/W7z6");

  // SHA-512.
  TEST_CRYPT("", "$6$mfq2p0FnRvKGQ5/F",
             "$lzgdWnWKb/5g/NauFOk6xUBYE0KWLLOA/"
             "WWXU8XGdPSurWarisqnDGxQBv.SmfvzRADwF3k9InkTMZp6ieEy./");
  TEST_CRYPT("R", "$6$ZNRt/aU7H3yFn9Zv",
             "$2cDCdCe.ja/eq6W7pftSqa6O/"
             "un1VfhSETY1xFov2J4LcXnkQYI9TLNNhGiCXXudIqST1zwsEcj239ghXTgsd/");
  TEST_CRYPT("pd", "$6$SodhEPJektwqfUIJ",
             "$8ioqnDeT4t0NDvDCyCRWQrFMHhELgwkZBpuqef59zZ2lLPm7SPOE2l/"
             "jRBCcpt65.h21.w67XGPkvRDFWUZGT1");
  TEST_CRYPT("QaA", "$6$UTRQXU9Sv.3Tlo/n",
             "$BUnJF45X73hC6OS45hS6B3y7bFXXBWEWSml."
             "qQvb2z3JwgeOsz6IVtAR5y7o7eZmF5.B2s723FgCiPIzd7r.Q0");
  TEST_CRYPT("Zu3U", "$6$9.G17Jyl2XTMGReX",
             "$/wHuiZBqUsFt.rx3HsLHVU/CjE8eStCh23aKip1nRjz1dGv17Do/kwBg6G.rr3/"
             "4uEdyRASMqUJCRWPU8rlA4.");
  TEST_CRYPT("gAbW8", "$6$vkM44b1rFVpSlZD8",
             "$nwd9K/vrtzukeHh/UuT3khe.x6cIh8scd4aOslOjwYRoZ6PP8RfptMW7gUa/"
             "thgDIAZQRfjenlJxOMB9cqyKV/");
  TEST_CRYPT("AH0QR3", "$6$895DOv3jlIASuVG4",
             "$IwBtMLpTkhP.."
             "SQ1SPyuVJhfzLedyZpgn10iWmNvsUlH40jdUEB7b1bXMwlmoPaVVvIN68bT4Ce2ko"
             "sLinkIY.");
  TEST_CRYPT("ogZRJDr", "$6$NttJsE0lah78x.JS",
             "$LP/"
             "dtJlHkhDG6bvhw9zvrPufdEzY3s8VH76yxIj0vm7wLOhESxdNoU8hkPBJpa8t8RMh"
             "uvgQQYcniXYo1v9ZM.");
  TEST_CRYPT("uWQHBFT9", "$6$Gvi1a/z44niX233y",
             "$U9vIkCVQkhZKjMQFovSJ6l1Q5adv."
             "l5WfpXcXbUbC4BdotxPsJHv30UDCCySZYsPnVxrAR2DoHpwQVF6qsBVu1");
  TEST_CRYPT("gL6kaMFV4", "$6$R7h/XtBAJJJ2j0wq",
             "$ymHLl7QbZnUUMOU6uwMmiR8DMHpM78PCXXSMv8JfzVc18Ae/"
             "H0zb60Kxln5.iJOne4lzxzHtvhOawql7XlL2S/");
  TEST_CRYPT("pc1PWD1vSE", "$6$Gu33txzA3sLpU8ZG",
             "$662v7aMqhHT4z9zmphM0PepQfIwCdQBDpAOXCopsImCfMT537VRT9SgI5refW7uT"
             "kSZ2Z47CM/bAUnYejOdMs0");
  TEST_CRYPT("n8lhfa9yIAq", "$6$VuWsx0ekQ8keV5Jm",
             "$BNAdbVbmlho3Tpb9yYo6F872s7SiKTQg1."
             "oZFo7dKNnJRT6859P6q01JFMdcK6lPD5u2HeiSEoYHwyaCVOuGz1");
  TEST_CRYPT("39YVq0aq0bc1", "$6$xM/1Utkak8uOv8/3",
             "$Da.aKJCROollPy2Z03aeW7NtwpufLG/"
             "026iOeZgqGSxUi58ve7QHKiFoX1Cew8Vmmhyxbo71Y2TmJoY0jf3F50");
  TEST_CRYPT("yN63we9QHMTFe", "$6$xtG1guxyXmD0GlO.",
             "$OAc1yf/"
             "AoSSIs50bOpjRaWU3STpRa3qKs3STUjG7Zy0OQxuh.kfbA9dCLlxhy."
             "WNVFcYiizuYJ070E0rDOv3d.");
  TEST_CRYPT("ShAdDNqrpfwMKZ", "$6$TDbP/smv5CG3ojCV",
             "$Zrozt.yClVvs5eipiQYvnLcgMzJ7s8Bp.MxoFrxcwD5e2ubqn4jlk4PQEwvQuo/"
             "yexncwn0yVlXEU0WDAJV4z.");
  TEST_CRYPT("KQdLY8fODbdIxLH", "$6$PhOseDcUwF/y5SxM",
             "$BZuC6iTziIsylp3ffZJBS2X807BEo/"
             "TQCGkVZO7IxccNss1XIkOiBpXnNZGibKLhq3y2C4L1lYEOIZ/XgLdZt.");
  TEST_CRYPT("K8x7psCL3uB83agb", "$6$4K8CoDUdijvflAgk",
             "$vsFzuLGYhNwaUkWMFV2C3tYo4/K9uso9ycZ4j9Ls04DONQh0c/"
             "Z3kEc9s0joTu0rCWqDu1PSz4OvPXIWHehWL/");
  TEST_CRYPT("cZiGBtOuXZEXnMfUr", "$6$0YNLCVys0kiFaG.4",
             "$X9ang4PmmyBpwX.r7tEPfh3riyOiT2ngilOPuv9/w7V4I1IzEyjmS9c/"
             "1b1SvD8zcTuzfdSkUGHF6wN0iGNuV1");
  TEST_CRYPT("cZUZVLwG8YpIEHRYFS", "$6$LtzTdAX1qAkbRaea",
             "$L1qbMQNKGJxeoXjG."
             "SWBuwE5hbCpjYnOLt13r8s9HiYllZXF59O6kHjV6sbkvFrdVFKopmbw."
             "gwdcoaNPnOL6.");
  TEST_CRYPT("DEiAvX2faslqVz8dmAn", "$6$PTf4DX48B92ntNIT",
             "$dCXc8aIksYLdmB2Rpt44WlTos7v/5UD3ay97JoKvJmvhWBJu/"
             "e6vghQVhgeXB9wvMPIFrJRkATNixse2TkFQp1");
  TEST_CRYPT("zergvaNB6ZKiqf8mEroS", "$6$2QHayGN2bx6n2WgZ",
             "$GSEUi/"
             "58jcQAH8ENRyEEbgxwOCadPgzhB2tiIKWCEAGeAeciRAx5JT.2EeI6fwaE1Z/"
             "rURynP96/7E52w8xte0");
  TEST_CRYPT("Y6UUDCidm1t8sM8VwSBg0", "$6$H/lOwNevWqurqHM.",
             "$lKU34oCVGgPJox2yLt."
             "8TZ0o716ynGK7Zr6qUVcb6fK6q6DqdmIgLTTofr31eXdB6TrhnNMF8AudeLKtMkjX"
             "t1");
  TEST_CRYPT("utXgv9tgvEanq5RQCr4oRO", "$6$q2YKQrQqJ5CAI3ng",
             "$H.vkLYHska9pYyaYDlw6ej04lGzd11F."
             "5G5XqwGVwK56VyIzqfnkl6xpS4zknkS234kikmMatlFyHPCwLCZiM0");
  TEST_CRYPT("xBAWRQS84md90exYSZSncxi", "$6$WND2TVU8YsBd23IA",
             "$cwbg4juign4NUkr80B3B2M.20LsVWXdmovhVN9u7n5YX3pLFid."
             "wGGaW0RgLchRghGyOlzluVvtwCyV03Z743/");
  TEST_CRYPT("AevsGlDNaYJ1U19BwTOF2YbX", "$6$z1h1FzasHt4InTVw",
             "$Bl/aH5msiIYIukYkfzxmqjsxsVoGOMN.RxdCuzpCe7ELx9QaJPa5QMcOkJj/"
             "bH8WFpHRDT4nbY/Tvqac86J6O/");
  TEST_CRYPT("dP86ohNlswN3SJNduTipwkwpg", "$6$Vargx67kDdTXkKL6",
             "$6FsO7e.oYPPtstVbx/"
             "ZLrubqyUBkceZu3nMVuldkkUhZ31vikETelRVcb6SqGr2CNMhdeILH6kYLWF0WWev"
             "qD.");
  TEST_CRYPT("McPchQvI2ndx0pmV0wdQUPGoqc", "$6$.6P0YyFdRDvb5HFI",
             "$cFgrAwNSNIksOofeHui6GojfKxFWJFgz7O4jYieTkY2izVR1T2sS4fOCwWBhbY92"
             "14QPHuZEj7Sm3.NVjPmPc.");
  TEST_CRYPT("cAA1w0A1Hbz9yR9ypiPQR2FlCtD", "$6$OQ.XuSltvM1brmc/",
             "$uWccwO0F8zY2c/UcWaJ.FZl3omckH7I1.Pih.CvRXYS/"
             "Wncl8m4gZm18VjO.VRCeKhF6H03C05bTbboIufwzA0");
  TEST_CRYPT("wtJanMovSQdOZ1bWMqdEQdTgOy3V", "$6$CJtx9ZL3WUtPO.IS",
             "$dkWUADIlfIs0WVa4w4dhjxn3zunFm40E107zy5hmGn9iDPbuFIcZ9iDtGdqlSLJi"
             "x.by11rsg4FBAgKz1eZaD1");
  TEST_CRYPT("N0tYSjbUMP11nKdcKiAjO8YwM2ddu", "$6$oZwY47Y8ywozjIS2",
             "$zYsjtH1rcMCuH/"
             "qr2FoSOW6HFO0uGyy9WQ8mGCQsyoapNFARwG1cuc9pEjXmyvXwRxE0RSgX9iZ8Kpd"
             "qvlnY.1");
  TEST_CRYPT("iEsos8ufraqyOLFc71DWcBeFQkysxw", "$6$PCLcl//bs3YrL8s6",
             "$0h0hh8AhCy8XTDgEMWEXB/YxlV/"
             "H.glwHy1fI4mc0zikkBhGMPtgXWmxm8j7io9WMBPqHllkAsxIQgLna0AYT0");
  TEST_CRYPT("asXhRNm9Wo35dv6xDmbsbEphMFUWq3H", "$6$jVS/2e67UieSdLju",
             "$45cAUyicuGBgtdUQl400b29/bkcmzjA1IK15.iW/tG61tvtO.3I1stFSXC4/"
             "xAT2p4ILbv62Om0D2kBTANtNk/");
  TEST_CRYPT("Jhj8vr6hFP895rBkHe77pcFjCpkxauxJ", "$6$59nXeADYcZHA3Sjn",
             "$D3wEI/QSc3ieeJHKoVRE2bWl/ikB6RTkMsrzqltcCAz8ygypupcssPWw0/"
             "xtd1pGs8a4KhUcj5DYJRQt3Hiof.");
  TEST_CRYPT("t0yFSqvpvwjtAQtINrfgUAZLbE7acx9bO", "$6$w0k68nclyv8HJ/Qv",
             "$Ku0j/AJGLNOk2H/"
             "ckSRKYSedFTUSCKnKubdLYf2czaCtCe7s.tLcRrsMDRaJ.TXj0qDg8JwGI9kW."
             "CBurZd/71");
  TEST_CRYPT("T5zFfgshwEESsndGDBTNb94HOIrUOFuxXo", "$6$ZMEZ8xDtAE.Ahnee",
             "$EI0Cq.F4nMfC23."
             "UbkC2a0YHfm1MqBy5B3eHzxYGILLnwkO7xO0tP4kixntSNbaIk12sx6mGux33h7LK"
             "6nFph.");
  TEST_CRYPT("4Eq2BgZ8Yaj9WsQPbI6p0HExkoNOMG2yeZv", "$6$Qo.hJCmg9qL7o6yb",
             "$qWQQdZTYAZofeBJJoJ4OgMc8FrkFIu0pPD7E77RCLAJv7qsP3szopBXXp3BlH5JM"
             "vMJ5uEbOs4ii2aSWcZHXl0");
  TEST_CRYPT("ryZGA7rJinUdorOfAEREwtBJXiu38Qf4l1ad", "$6$ehQPk1ofAGjd19B0",
             "$bOyx1qy4xcVs/"
             "KEsI5GeYMzx24jjqd3DtIItEJYyTU."
             "DjYNlOljt2Vg2T4sJlcxXxY2QVpdI3f3mwD218UEA.1");
  TEST_CRYPT("uk6sRtOW3R0AWPVzovL5M8F1lh34peWoXbzS5", "$6$j1wT3kdCrbKAsjTn",
             "$yHlrXZZne41nj8maA93C7a6QORk5jqQKUZKZVJcsQSwSfOB4.zoCPU.VBPUk/"
             "x32IRyt8cZYL7HZz2n/NMvvT0");
  TEST_CRYPT("S6Rrwc3MSwZ7Euhdi4B8TDZYaOhLcBONBmZez5", "$6$CUNenV.jfmot/S1P",
             "$MiXfF6nW6TpOgsYamq2g356xQDIJMcdiHkYMJjPH34JgU0XayLcu1rlaaNt53Yeu"
             "Cysui7XOaVTpz58y4Jjsn0");
  TEST_CRYPT("Flfn9voWXicQOFVWmBSdngd6VMkGh8S0lK3PPBP", "$6$WDwd/Q8ZYxSdG8P5",
             "$g5D575ebW27d19b5EhyJo9pjcXJp6/If6A/Dm/M5f0Asf6BcN509bIpea/"
             "k8XB2jPJOkwxBxn4UFlels7BrNY.");
  TEST_CRYPT("jhEETQTpJ2LsypfWBecXpYB6j2MUb84PjCtn7BRY", "$6$Q5OQbVdx3qx1zCZ1",
             "$1Ed3CaiK/"
             "hgYus7ZZM3FqcLKK4yG0ejVOcqVflfCMbQ3pN2SHM9SjPVnkM980g9YRhZ/"
             "5XEEwPPtmO9F6Udo5/");
  TEST_CRYPT("WPS6PHy7sPnYd6lKXokpewrAY1cFC9PabwJOd9UUt", "$6$CZPQC9r0BonBX50R",
             "$TAdIZ7xpMRb7oGWhUVx1Sr0hpgKbN97QCsLMCMARGw."
             "tP6bix3Tk16xufKwM80hVEihoGcoIbii1TTCA0T136/");
  TEST_CRYPT("eKM69x7ckSlmYmOVbmt0knUUAg00vU4ZmvIDgzXQQ0",
             "$6$LH9aZsqzhGeYJZxx",
             "$S6CcUhOAKrchCvPaINtz3v94iLFMuYBgzC2YP9SPCfOtoZhXwFxKMSzma92IHfDr"
             "WNDTImpJQaVXvzS09l7o2.");
  TEST_CRYPT("dZZwPviIJ5pDK7XbnSETG6UPO20tHDa4PysDp3qRE9a",
             "$6$O6CLuufSOxmO/OH3",
             "$r/lm2nygSmIR/0ET514tCik9t3GQ0FjCTsWp/"
             "NQBS3l6gKR.MEjQqLU948C5wyQSI3J/suJZFjUJ4IFHp7yKT0");
  TEST_CRYPT("7sfOXWEntTZGDyWGHcBAkjIlUlKcxcRPaQhyP0h5Dkwr",
             "$6$E69ypDj5nQ.DfxLS",
             "$UtETLrErC6da0yc0H.Pvs87kU9I1WagXj22snpnZm2GtlysrrUUY3Tm9CCRD/"
             "fmW5ghvZXj73fkSMhKjaI7q0/");
  TEST_CRYPT("4alYzNGAaoXZcb1mrRdO1XiK5TyNZGQHYqFHqj8BmNl7u",
             "$6$odJVOIORRZ5mPOFt",
             "$83iMe6O4TjhTmuDsXv6VVNc0EM.Bw/8Zv64NJ1e4nAt6/"
             "pOw2ir8HNFOAylWdDnFQDeMJvK36eosmb18hwM3t.");
  TEST_CRYPT("05dex7FjN84XELGJFEkqUyRj0zGUeg8L1ptW5HhaPrTFmk",
             "$6$WMeGKDUUbgWpBXlX",
             "$XHGq91hevGz8VwbCoG4234hOzIKLdRPfGQ6/"
             "riyUiPsBZtDnFTAATZag0fpCcLD4280d1BQJmyc3Q6gP0QIId1");
  TEST_CRYPT("i3Yjrehzd2n6ro7AQnRMvHvsoLs9y0Cvk5HNdMeUpAM5FXv",
             "$6$.x/aYQC5.5LX1w/E",
             "$Ebz6tAHTn/PXGw3wa6SGb9alQg5iskylyvBt2kgzPtcWla3D9AeTxEbk/"
             "UPheaE1CYlQvWqJKJt/JWO8ACkqN1");
  TEST_CRYPT("2jEdBjztdAv9l2aSzUSmsA5BoQ6p2zzWUzgpNzY7CvBuZjs4",
             "$6$qO/dvWtfI2LQ5w/J",
             "$eb8bJ.pppeJDomQm54m/xni6tgQSCs0B2040t9GIUK3BUr7M/"
             "0Ygjs7nBjVNTRsiF38bF4./j/lhhwY2BZmTi1");
  TEST_CRYPT("dqAnp30wb4tdDPUl2lVTb3ZI836fJTxKafK6GoVJP7mPtuqku",
             "$6$ZLzEI7R8oS3IIu6Z",
             "$greVadq9ZuPEjDYax1IkE7oy1EDMfI.g/"
             "0y89NtOjUy0ouLkmMgV6AY1W5GnybMbrJjiPiqQkVI/KjzSl9/Nv1");
  TEST_CRYPT("2qIwaSbi8FPHng8EFJSKGS45xc3EixxmyBVuNpv5joBo1k5dNs",
             "$6$X2B7Zi5.2SldhhOT",
             "$TAiiLopYtLlJfJ0BiVVxBwfsHHCMWKfdyL0a3nsxpvN4To8kPcu9woEl3P6gRlEV"
             "VN/MveIRJkP3g4I6Dmhr6/");
  TEST_CRYPT("h6egLiAd2XnVKiePoST7yDu1B9FgOXzy4N639ll273aMz3R6uxP",
             "$6$X5Jb/AypSFix6bod",
             "$CF1Zuo/FuD/"
             "YUKa7xdJJ.7F4SMXSOl5aRY9YEAP.lcuG1."
             "Q49tqN29I1ea599Dyp9c77jDELs5nkQ0UsjzBiZ/");
  TEST_CRYPT("1dAkzIGW5n5kWnh0HD7X2daMIsNMu9bbhxauAzTbj1RVCN1dGh14",
             "$6$k.eKR9etRaRkTyRu",
             "$MC9r6BKyj7PxupnJVywZqCbDRMUHUlJn90Y0usZRWhF0JRtB7YnIPJQYWd2ry3WT"
             "tZhs9ZcdL9h.A/1lUWsju1");
  TEST_CRYPT("wtgu8B2ypBw8FbogGZiH9dhobMpcIMHPcn1TCzDfJgTVGyqylS0hL",
             "$6$FF5.sURcPLWY0J77",
             "$yTYnOpRmMMJrjvvYZT6.dCiz6BIftAZxVr3MAU4sGThy/"
             "KbS0Gwpaf9p9e8ZGN35bH2UslLZqfJ.Xd8mGAjjS/");
  TEST_CRYPT("92Ljf3i7r0z87qjHVT7k54s5fQCwMtoyYcoh6FHtrBJlMb5thRkVcN",
             "$6$qds6dfiTiwWaPokK",
             "$Yg6.Bxf3v/fBDB8aR2E72ntRpj8byZoXh1f0imp1l/"
             "tIMAmZVazzbjxn8FMWMHgtgJylG97LUDDEqN6if0Yon/");
  TEST_CRYPT("dkqliMrZKfuuvFLNpEmjWyKBas2rCuM0fMHMOwQmzKRzMM7a4P8KXdd",
             "$6$9ZY0mu7hyC.mdGLW",
             "$g9rF02DJvn0HosSyWew.mGT02vqOaVRoeOKN."
             "kUC1tLo07ANQSrWo4O2v2qS5frQE/F4o1M7x5QlysZPD70/8/");
  TEST_CRYPT("wYXBun7gq2IRCmkZedDWBpLAr0LDhikvOAgi33BfFtwgQ6aB15nX9op8",
             "$6$0blMZshOzzBtTScE",
             "$UqnNyvRnhiJhOD5."
             "JZX0EaojgHZYkS1gmvM5EdmZkLo3CPDm6o8SaRrKbgOw6eiVZP0Bvw6Kjd9uuIGEU"
             "1Rm70");
  TEST_CRYPT("yeiO6UBM4sjhGw0nL9I0iLscKotO6K8Z3ehNpIEJsYnguh3vb6R21JmbQ",
             "$6$dwDGqmQcZi19dn7h",
             "$pnft8hEtq4ipshl74v3EDezuFILvMJdRxmNlZVDIcDNeiHYcOAnLrYMA7t0uCTGG"
             "YvpnPUKprm8DP.4bqsjDv/");
  TEST_CRYPT("p9jNNqvqo1PD40FRQ3Ut9fZkzKuiSSL9Ip10PyVtOlDPcXGmqnu64drxZD",
             "$6$huih4OoCetgMQHJX",
             "$b9hhpvaBxkU2k3DgtS.ibAg7QIatx2lvihSPV24DYFyknQLLpLd."
             "0aXHCdzegeM8xlhrx5oEXbeTB/jlgR3rl0");
  TEST_CRYPT("k1v05zoe52mMX1r94FJ0Lfkh1Rj261MsuSGgtk08os8ca52r9PhmBDZC0IL",
             "$6$dS79bajoHjyy6jlr",
             "$ptytz/NVsIrHwegkxl8ma82DVGshb6uVKHNysO4kD/"
             "5JmQagAd7m8c29DK5Q5PIEFQaHk.x.hRzd/V/szaP3Z.");
  TEST_CRYPT("uIIAZSBK03wsJitQkioC80TrdQQyRCejx5drh0yj8q9PiOrnSUe4rUPTrxdS",
             "$6$7buz6rgW.k87qJAb",
             "$Cp9YkmHpS3s.m8B22jmLv1LLUFVTex0sA3d9rBLV0akyoE1kPx2qsMl8xD/.N/"
             "YWTxvLmADoaji9HwJ7e.alP/");
  TEST_CRYPT("xEUkyxYRE03EPxG3ymuD4MGhvTjZSN0suQ7xCc7bgz6pyHPDIoz0w3qKQ2Ve0",
             "$6$OJUs6GL4vuGzM4nc",
             "$QPk/"
             "fVPbHaqlT6mliRz9MD01OYdz8jeBGj9DwNTJ3."
             "KEJthni77sh8Sh5h86gZLCNzPJPCEhvRt9Wn/DCMEPs0");
  TEST_CRYPT("msKCywJHNkC7EO5OgalChfLbYCEe5vhbSM2hQpRqEvaSU5dRo4rW6Mdi6nh9Sp",
             "$6$TYYtXT7nF1XZXxB9",
             "$7.8rVSRvlNLSPBzv/XSWmGuuYB6rfywxg8DfX4/4i/z/"
             "WfyT7zXj8sLBnDURTx7i06Tk9Pnp1Km1/X2CSM0RA1");
  TEST_CRYPT("Lka7evCL2zYIx7FQMlmIUc0kKTlmn2AEk9i0fq048NvFqQtmJKrfn4Ibc1jqsRK",
             "$6$C41LUG50LcszJFSN",
             "$CBjccf.JMnS/"
             "OGz8MAI7J6hZfDTYbKhDkBQyp6TjLWybh."
             "pcVnrvkqxWHua2nHwNjjWMV4PhqSddN.uBvfk.f/");

  // Custom number of rounds.
  TEST_CRYPT("Hello", "$6$rounds=1234$World",
             "$jdCCRAsVFZxDmpUraC6x6nJypNALDJAc34JmFuPgAZhmhcukx8LGLuDuVbU5WIEz"
             "j/HS/w4OM0nAeo5lttBAX1");
#undef TEST_CRYPT
}
