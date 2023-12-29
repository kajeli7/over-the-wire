Napi::Object InitSystemLocal(Napi::Env env, Napi::Object exports) {
  ENUM_VALUE(AF_UNIX);      //  Local communication                        unix(7)
  ENUM_VALUE(AF_LOCAL);     //  Synonym for AF_UNIX
  ENUM_VALUE(AF_INET);      //  IPv4 Internet protocols                    ip(7)
  ENUM_VALUE(AF_IPX);       //  IPX - Novell protocols
  ENUM_VALUE(AF_APPLETALK); //  AppleTalk                                  ddp(7)
  ENUM_VALUE(AF_INET6);     //  IPv6 Internet protocols                    ipv6(7)
  ENUM_VALUE(AF_DECnet);    //  DECet protocol sockets


  ENUM_VALUE(SOCK_STREAM);
  ENUM_VALUE(SOCK_DGRAM);
  ENUM_VALUE(SOCK_SEQPACKET);
  ENUM_VALUE(SOCK_RAW);
  ENUM_VALUE(SOCK_RDM);

  ENUM_VALUE(PF_UNIX);
  ENUM_VALUE(PF_LOCAL);
  ENUM_VALUE(PF_INET);
  ENUM_VALUE(PF_IPX);
  ENUM_VALUE(PF_APPLETALK);
  ENUM_VALUE(PF_INET6);
  ENUM_VALUE(PF_DECnet);
  ENUM_VALUE(PF_KEY);

  ENUM_VALUE(SO_DEBUG);
  ENUM_VALUE(SO_REUSEADDR);
  ENUM_VALUE(SO_REUSEPORT);
  ENUM_VALUE(SO_KEEPALIVE);
  ENUM_VALUE(SO_DONTROUTE);
  ENUM_VALUE(SO_LINGER);
  ENUM_VALUE(SO_BROADCAST);
  ENUM_VALUE(SO_OOBINLINE);
  ENUM_VALUE(SO_SNDBUF);
  ENUM_VALUE(SO_RCVBUF);
  ENUM_VALUE(SO_SNDLOWAT);
  ENUM_VALUE(SO_RCVLOWAT);
  ENUM_VALUE(SO_SNDTIMEO);
  ENUM_VALUE(SO_RCVTIMEO);
  ENUM_VALUE(SO_TYPE);
  ENUM_VALUE(SO_ERROR);
  ENUM_VALUE(SO_NOSIGPIPE);

  ENUM_VALUE(IPPROTO_IP);
  ENUM_VALUE(IPPROTO_HOPOPTS);
  ENUM_VALUE(IPPROTO_ICMP);
  ENUM_VALUE(IPPROTO_IGMP);
  ENUM_VALUE(IPPROTO_IPIP);
  ENUM_VALUE(IPPROTO_TCP);
  ENUM_VALUE(IPPROTO_EGP);
  ENUM_VALUE(IPPROTO_PUP);
  ENUM_VALUE(IPPROTO_UDP);
  ENUM_VALUE(IPPROTO_IDP);
  ENUM_VALUE(IPPROTO_TP);
  ENUM_VALUE(IPPROTO_IPV6);
  ENUM_VALUE(IPPROTO_ROUTING);
  ENUM_VALUE(IPPROTO_FRAGMENT);
  ENUM_VALUE(IPPROTO_RSVP);
  ENUM_VALUE(IPPROTO_GRE);
  ENUM_VALUE(IPPROTO_ESP);
  ENUM_VALUE(IPPROTO_AH);
  ENUM_VALUE(IPPROTO_ICMPV6);
  ENUM_VALUE(IPPROTO_NONE);
  ENUM_VALUE(IPPROTO_DSTOPTS);
  ENUM_VALUE(IPPROTO_MTP);
  ENUM_VALUE(IPPROTO_ENCAP);
  ENUM_VALUE(IPPROTO_PIM);
  ENUM_VALUE(IPPROTO_SCTP);
  ENUM_VALUE(IPPROTO_RAW);

  return exports;
  return exports;
}
