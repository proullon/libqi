/*
 * Copyright (c) 2012, 2013 Aldebaran Robotics. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the COPYING file.
 */


#include <boost/filesystem.hpp>
#include <locale>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <unistd.h> //gethostname
#include <algorithm>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <limits.h>

# include <pwd.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
#ifndef ANDROID
# include <ifaddrs.h>
#endif

#if defined (__linux__)
# include <sys/prctl.h>
#endif


#include <pthread.h>
#include <qi/os.hpp>
#include <qi/log.hpp>
#include <qi/error.hpp>
#include <qi/qi.hpp>
#include "filesystem.hpp"
#include "utils.hpp"

namespace qi {
  namespace os {

    FILE* fopen(const char *filename, const char *mode) {
      return ::fopen(boost::filesystem::path(filename, qi::unicodeFacet()).string(qi::unicodeFacet()).c_str(),
                     boost::filesystem::path(mode, qi::unicodeFacet()).string(qi::unicodeFacet()).c_str());

    }

    int stat(const char *filename, struct ::stat* status) {
      return ::stat(boost::filesystem::path(filename, qi::unicodeFacet()).string(qi::unicodeFacet()).c_str(), status);
    }

    std::string getenv(const char *var) {
      char *res = ::getenv(boost::filesystem::path(var, qi::unicodeFacet()).string(qi::unicodeFacet()).c_str());
      if (res == NULL)
        return "";
      return std::string(res);
    }

    int setenv(const char *var, const char *value) {
      return ::setenv(boost::filesystem::path(var, qi::unicodeFacet()).string(qi::unicodeFacet()).c_str(),
                      boost::filesystem::path(value, qi::unicodeFacet()).string(qi::unicodeFacet()).c_str(), 1);
    }

    void sleep(unsigned int seconds) {
      // In case sleep was interrupted by a signal,
      // keep sleeping until we have slept the correct amount
      // of time.
      while (seconds != 0) {
        seconds = ::sleep(seconds);
      }
    }

    void msleep(unsigned int milliseconds) {
      // Not the same for usleep: it returns a non-zero
      // error code if it was interupted...
      usleep(milliseconds * 1000);
    }


    std::string home()
    {
      std::string envHome = qi::os::getenv("HOME");
      if (envHome != "")
      {
        return boost::filesystem::path(envHome, qi::unicodeFacet()).make_preferred().string(qi::unicodeFacet());
      }

      // $HOME environment variable not defined:
      char *lgn;
      struct passwd *pw;
      if ((lgn = getlogin()) != NULL && (pw = getpwnam(lgn)) != NULL)
      {
        return boost::filesystem::path(pw->pw_dir, qi::unicodeFacet()).make_preferred().string(qi::unicodeFacet());
      }

      // Give up:
      return "";
    }

    std::string mktmpdir(const char *prefix)
    {
      std::string sprefix;
      std::string tmpdir;
      std::string path;
      int         i = 0;

      if (prefix)
        sprefix = prefix;

      bool isCreated = false;
      do
      {
        tmpdir = sprefix;
        tmpdir += randomstr(7);
        boost::filesystem::path pp(qi::os::tmp(), qi::unicodeFacet());
        pp.append(tmpdir, qi::unicodeFacet());
        path = pp.make_preferred().string(qi::unicodeFacet());
        ++i;

        try
        {
          isCreated = boost::filesystem::create_directory(pp.make_preferred());
        }
        catch (const boost::filesystem::filesystem_error &e)
        {
          qiLogDebug("qi::os") << "Attempt " << i << " fail to create tmpdir! " << e.what();
        }
      }
      while (i < TMP_MAX && !isCreated);

      return path;
    }

    std::string tmp()
    {
      std::string temp = ::qi::os::getenv("TMPDIR");
      if (temp.empty())
        temp = "/tmp/";

      boost::filesystem::path p = boost::filesystem::path(temp, qi::unicodeFacet());

      return p.string(qi::unicodeFacet());
    }

    int gettimeofday(qi::os::timeval *tp) {
      struct ::timeval tv;
      int ret = ::gettimeofday(&tv, 0);
      tp->tv_sec = tv.tv_sec;
      tp->tv_usec = tv.tv_usec;
      return ret;
    }

    std::string tmpdir(const char *prefix)
    {
      return mktmpdir(prefix);
    }

#ifdef ANDROID
    std::string gethostname()
    {
      assert(0 && "qi::os::gethostname is not implemented for android, "
                  "use the JAVA API instead");
      return "";
    }
#else
    std::string gethostname()
    {
      long hostNameMax;
#ifdef HAVE_SC_HOST_NAME_MAX
      hostNameMax = sysconf(_SC_HOST_NAME_MAX) + 1;
#else
      hostNameMax = HOST_NAME_MAX + 1;
#endif
      char *szHostName = (char*) malloc(hostNameMax * sizeof(char));
      memset(szHostName, 0, hostNameMax);
      if (::gethostname(szHostName, hostNameMax) == 0) {
        std::string hostname(szHostName);
        free(szHostName);
        return hostname;
      }

      free(szHostName);
      return std::string();
    }
#endif

    char* strdup(const char *src)
    {
      return ::strdup(src);
    }

    int snprintf(char *str, size_t size, const char *format, ...)
    {
      va_list list;
      va_start(list, format);
      int ret = vsnprintf(str, size, format, list);
      va_end(list);

      return ret;
    }

    unsigned short findAvailablePort(unsigned short port)
    {
      struct sockaddr_in name;
      name.sin_family = AF_INET;
      name.sin_addr.s_addr = htonl(INADDR_ANY);
      int sock = ::socket(AF_INET, SOCK_STREAM, 0);

      // cast ushort into int to check all ports between
      // [49152, 65535] (e.g. USHRT_MAX)
      unsigned short iPort = port != 0 ? port : static_cast<unsigned short>(49152);
      int unavailable = -1;

      do
      {
        name.sin_port = htons(iPort);
        unavailable = ::bind(sock, (struct sockaddr *)&name, sizeof(name));

        if (!unavailable)
        {
          unavailable = ::close(sock);
          if (!unavailable)
            break;
        }
        ++iPort;
      }
      while (iPort + 1 > USHRT_MAX);

      if (unavailable)
      {
        iPort = 0;
        qiLogError("core.common.network") << "findAvailablePort Socket Cannot find available port, Last Error: "
                                          << unavailable << std::endl;
      }
      qiLogDebug("core.common.network") << "findAvailablePort: Returning port: "
                                        << iPort << std::endl;
      return iPort;
    }

#ifdef ANDROID
    std::map<std::string, std::vector<std::string> > hostIPAddrs(bool ipv6Addr)
    {
      qiLogWarning("libqi.hostIPAddrs") << "qi::os::hostIPAddrs is partially implemented on Android: Only return the loopback address.";
      std::map<std::string, std::vector<std::string> > res;
      std::vector<std::string> addrs;
      addrs.push_back("127.0.0.1");
      res["lo"] = addrs;
      return res;
    }
#else
    std::map<std::string, std::vector<std::string> > hostIPAddrs(bool ipv6Addr)
    {
      std::map<std::string, std::vector<std::string> > ifsMap;
      struct ifaddrs *ifAddrStruct = 0;
      struct ifaddrs *ifa = 0;
      void *tmpAddrPtr = 0;
      int ret = 0;

      ret = getifaddrs(&ifAddrStruct);
      if (ret == -1) {
        qiLogError("getifaddrs") << "getifaddrs failed: " << strerror(errno);
        return std::map<std::string, std::vector<std::string> >();
      }

      for (ifa = ifAddrStruct; ifa != 0; ifa = ifa->ifa_next)
      {
        if (!ifa->ifa_addr)
          continue;
        if (ifa->ifa_addr->sa_family == AF_INET && !ipv6Addr)
        {
          tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
          char addressBuffer[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

          ifsMap[ifa->ifa_name].push_back(addressBuffer);
        }
        else if (ifa->ifa_addr->sa_family == AF_INET6 && ipv6Addr)
        {
          tmpAddrPtr = &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
          char addressBuffer[INET6_ADDRSTRLEN];
          inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
          ifsMap[ifa->ifa_name].push_back(addressBuffer);
        }
      }
      freeifaddrs(ifAddrStruct);
      return ifsMap;
     }
#endif

    void setCurrentThreadName(const std::string &name) {
     #if defined (__linux__) && !defined(ANDROID)
      prctl(PR_SET_NAME, name.c_str(), 0, 0, 0);
     #elif defined (__APPLE__)
      pthread_setname_np(name.c_str());
     #elif defined (ANDROID)
      //no implementation under Android
     #else
      //BSD, whatever...
      pthread_set_name_np(pthread_self(), name.c_str());
     #endif
    }

    //true on success
    bool setCurrentThreadCPUAffinity(const std::vector<int> &cpus) {
     #if defined (__linux__) && !defined(ANDROID)
      cpu_set_t cpu;
      pthread_t self = pthread_self();
      CPU_ZERO(&cpu);
      for (unsigned int i = 0; i < cpus.size(); ++i)
        CPU_SET(cpus[i], &cpu);
      int ret = 0;
      ret = pthread_setaffinity_np(self, sizeof(cpu_set_t), &cpu);
      return !ret;
     #endif
      return false;
    }

  }
}
