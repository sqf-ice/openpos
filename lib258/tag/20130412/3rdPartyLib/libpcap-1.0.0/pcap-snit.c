/*
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1995, 1996
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Modifications made to accommodate the new SunOS4.0 NIT facility by
 * Micky Liu, micky@cunixc.cc.columbia.edu, Columbia University in May, 1989.
 * This module now handles the STREAMS based NIT.
 */

#ifndef lint
static const char rcsid[] _U_ =
    "@(#) $Header: /tcpdump/master/libpcap/pcap-snit.c,v 1.73.2.4 2008-04-14 20:41:52 guy Exp $ (LBL)";
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/dir.h>
#include <sys/fcntlcom.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stropts.h>

#include <net/if.h>
#include <net/nit.h>
#include <net/nit_if.h>
#include <net/nit_pf.h>
#include <net/nit_buf.h>

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <netinet/ip_var.h>
#include <netinet/udp.h>
#include <netinet/udp_var.h>
#include <netinet/tcp.h>
#include <netinet/tcpip.h>

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "pcap-int.h"

#ifdef HAVE_OS_PROTO_H
#include "os-proto.h"
#endif

/*
 * The chunk size for NIT.  This is the amount of buffering
 * done for read calls.
 */
#define CHUNKSIZE (2*1024)

/*
 * The total buffer space used by NIT.
 */
#define BUFSPACE (4*CHUNKSIZE)

/* Forwards */
static int nit_setflags(int, int, int, char *);

static int
pcap_stats_snit(pcap_t *p, struct pcap_stat *ps)
{

	/*
	 * "ps_recv" counts packets handed to the filter, not packets
	 * that passed the filter.  As filtering is done in userland,
	 * this does not include packets dropped because we ran out
	 * of buffer space.
	 *
	 * "ps_drop" counts packets dropped inside the "/dev/nit"
	 * device because of flow control requirements or resource
	 * exhaustion; it doesn't count packets dropped by the
	 * interface driver, or packets dropped upstream.  As filtering
	 * is done in userland, it counts packets regardless of whether
	 * they would've passed the filter.
	 *
	 * These statistics don't include packets not yet read from the
	 * kernel by libpcap or packets not yet read from libpcap by the
	 * application.
	 */
	*ps = p->md.stat;
	return (0);
}

static int
pcap_read_snit(pcap_t *p, int cnt, pcap_handler callback, u_char *user)
{
	register int cc, n;
	register u_char *bp, *cp, *ep;
	register struct nit_bufhdr *hdrp;
	register struct nit_iftime *ntp;
	register struct nit_iflen *nlp;
	register struct nit_ifdrops *ndp;
	register int caplen;

	cc = p->cc;
	if (cc == 0) {
		cc = read(p->fd, (char *)p->buffer, p->bufsize);
		if (cc < 0) {
			if (errno == EWOULDBLOCK)
				return (0);
			snprintf(p->errbuf, sizeof(p->errbuf), "pcap_read: %s",
				pcap_strerror(errno));
			return (-1);
		}
		bp = p->buffer;
	} else
		bp = p->bp;

	/*
	 * loop through each snapshot in the chunk
	 */
	n = 0;
	ep = bp + cc;
	while (bp < �%:#& hh  �`����%2#& hi  �P����%*#& hj  �@����%"#& hk  �0����%#& hl  � ����%#& hm  �����%
#& hn  � ����%#& ho  ������%�"& hp  ������%�"& hq  ������%�"& hr  ������%�"& hs  �����%�"& ht  �����%�"& hu  �����%�"& hv  �����%�"& hw  �p����%�"& hx  �`����%�"& hy  �P����%�"& hz  �@����%�"& h{  �0����%�"& h|  � ����%�"& h}  �����%�"& h~  � ����%�"& h  ������%z"& h�  ������%r"& h�  ������%j"& h�  ������%b"& h�  �����%Z"& h�  �����%R"& h�  �����%J"& h�  �����%B"& h�  �p����%:"& h�  �`����%2"& h�  �P����%*"& h�  �@����%""& h�  �0����%"& h�  � ����%"& h�  �����%
"& h�  � ����%"& h�  ������%�!& h�  ������%�!& h�  ������%�!& h�  ������%�!& h�  �����%�!& h�  �����%�!& h�  �����%�!& h�  �����%�!& h�  �p����%�!& h�  �`����%�!& h�  �P����%�!& h�  �@����%�!& h�  �0����%�!& h�  � ����%�!& h�  �����%�!& h�  � ����%�!& h�  ������%z!& h�  ������%r!& h�  ������%j!& h�  ������%b!& h�  �����%Z!& h�  �����%R!& h�  �����%J!& h�  �����%B!& h�  �p����%:!& h�  �`����%2!& h�  �P����%*!& h�  �@����%"!& h�  �0����%!& h�  � ����%!& h�  �����%
!& h�  � ����%!& h�  ������%� & h�  ������%� & h�  ������%� & h�  ������%� & h�  �����%� & h�  �����%� & h�  �����%� & h�  �����%� & h�  �p����%� & h�  �`����%� & h�  �P����%� & h�  �@����%� & h�  �0����%� & h�  � ����%� & h�  �����%� & h�  � ����%� & h�  ������%z & h�  ������%r & h�  ������%j & h�  ������%b & h�  �����%Z & h�  �����%R & h�  �����%J & h�  �����%B & h�  �p����%: & h�  �`����%2 & h�  �P����%* & h�  �@����%" & h�  �0����% & h�  � ����% & h�  �����%
 & h�  � ����% & h�  ������%�& h�  ������%�& h�  ������%�& h�  ������%�& h�  �����%�& h�  �����%�& h�  �����%�& h�  �����%�& h�  �p����%�& h�  �`����%�& h�  �P����%�& h�  �@����%�& h�  �0����%�& h�  � ����%�& h�  �����%�& h�  � ����%�& h�  ������%z& h�  ������%r& h�  ������%j& h�  ������%b& h�  �����%Z& h�  �����%R& h�  �����%J& h�  �����%B& h�  �p����%:& h�  �`����%2& h�  �P����%*& h�  �@����%"& h�  �0����%& h�  � ����%& h�  �����%
& h�  � ����%& h�  ������%�& h�  ������%�& h�  ������%�& h�  ������%�& h�  �����%�& h�  �����%�& h�  �����%�& h�  �����%�& h�  �p����%�& h�  �`����%�& h�  �P����%�& h�  �@����%�& h�  �0����%�& h�  � ����%�& h�  �����%�& h�  � ����%�& h�  ������%z& h   ������%r& h  ������%j& h  ������%b& h  �����%Z& h  �����%R& h  �����%J& h  �����%B& h  �p����%:& h  �`����%2& h	  �P����%*& h
  �@����%"& h  �0����%& h  � ����%& h  �����%
& h  � ����%& h  ������%�& h  ������%�& h  ������%�& h  ������%�& h  �����%�& h  �����%�& h  �����%�& h  �����%�& h  �p����%�& h  �`����%�& h  �P����%�& h  �@����%�& h  �0����%�& h  � ����%�& h  �����%�& h  � ����%�& h  ������%z& h   ������%r& h!  ������%j& h"  ������%b& h#  �����%Z& h$  �����%R& h%  �����%J& h&  �����%B& h'  �p����%:& h(  �`����%2& h)  �P����%*& h*  �@����%"& h+  �0����%& h,  � ����%& h-  �����%
& h.  � ����%& h/  ������%�& h0  ������%�& h1  ������%�& h2  ������%�& h3  �����%�& h4  �����%�& h5  �����%�& h6  �����%�& h7  �p����%�& h8  �`����%�& h9  �P����%�& h:  �@����%�& h;  �0����%�& h<  � ����%�& h=  �����%�& h>  � ����%�& h?  ������%z& h@  ������%r& hA  �����H�i� PH�5�} H�=j �H  �`���H��� RH�5�} H�=L~ ��  �@���H�5i} QH�=>~ H�ʁ �{   � ���H��H� & H��t��H��Ð��������U�=�%&  H��ATSubH�=� &  tH�=�& �*���H���% L�%��% H��%& L)�H��H��H9�s D  H��H��%& A��H��%& H9�r���%& [A\]�f�     H�=H�%  UH��tH�s�% H��t]H�=.�% ��@ ]Ð�����H��SH��tH�H��t�� H��[����[�fD  L��� H��� H�5�� H�=�� ��   ����f�H�\$�H�l$�H��L�d$�H��H���B���H9H��� t&H�$H�l$H�5� L�d$H�=W� H�������H������H��I��t`�   ����H�0� L��   H��H���<���H��t'H��H��H�$H�l$L�d$H������f.�     H��1��&�����@ H��� �^���@ SH����H�2$& H��t[� H�=!$& �L�����t��� H�=Է �����H��� H��� H�5� H�=�� A�   ��#& ����H��H������H��� H��� H�5� H�=� A�   �X���H��H���]���H��� H��� H�5.� H�=.� A�   �&���H��H���+��������H��� H�5x H�=A� A�   H������H��H�������H�� H�޴ H�5� H�=ߴ A�   �����H��H�������H��� H�˴ H�5@� H�=̴ A�   ����H��H����������H��� H�5�� H�=�� A�   H�������H��H���a���[H�=�"& �   ����ffffff.�     H��H�}"& H��u<H�9�% H�5e� 1ɿ   �6���H�W"& �:���H�=K"& H���+���H�<"& H���ffffff.�     H���w���H�0& H�!& �T���H��& H�& H��ÐUH��SH��H������H��tH�H��tH9t5H��H���������u&H�γ H�5� H�=6� �����H��1�[]� �c���H�;H������H�x t)�L���H�;H������H�@H��H��[H��]���    H��� H�5z� H�=в �~���H��1�[]�D  H�l$�H�\$�H��L�d$�L�l$�H��8�����H��tH�U H��tH9t=H��H���"�����u.L�� H�P� H�5T� H�=_� ��   �����     H�t$H�������H��I����   �� & �P   H�������H�ǉ������H��H�L$t
H9H��   H�=��% H�5�� L���`� H��I����   �    �*���H��L� H�D$D�%H & �P   H��L�kH�C����H�����H��H��D������H��H�\$H�l$ L�d$(L�l$0H��8�f�L9 �q�����D  H�XJ H�5B� H�=X� ����1��f�H�a� H�=?� 1��   �����1��f�SH���g���H��t*H�xH�5ұ H�C� 1�1��� H��H��[��  H��� H�5�� H�=� ����1�[�f.�     SH������H��t*H�xH�5r� H�� 1�1��7� H��H��[�[�  H�K� H�5ʵ H�=�� �.���1�[�f.�     H�\$�H�l$�H��L�d$�H��(������H��tH�H��tH9t9H��H���������u*L��� H��� H�5� H�=� �  �l���@ H�t$H������H��I��t;������H��H��tTH�T$L��H�������H��H�l$H�\$L�d$ H��(�D  H��H H�5
� H�=�� 1��L�����f.�     H�� H�5� H�=x� �&����@ UH��SH��H������H��t#H�H��tH9tH��H����������    HD��r���H��t$H�U H��tH9tH��H���������    HD�H9�   t^H��tYH��teD  �+���H��H������H��H���u���H������H��H������H�t$H���U���H�L$H�4$H��H���A���H��[]�f.�     �������f�     UH��SH��H������H��tH�U H��tH9t<H��H���������u-H�ͮ H�5�� H�=5� �����H��1�[]�f.�     H���X���H��t;H�xH�5î H�ˮ H�L� E1�1�耾 H��H��H��[]�?  �    H��� H�5�� H�=�� �n���H��1�[]�D  H�5� �����@ UH��SH��H�������H��tH�H��tH9t5H��H��������u&H�� H�5�� H�=V� ����Z[1�]�fD  H��t#H��H���h���1�H��H��t�u H��[]����H�� H�52� H�=� �����@ UH��SH��H���/���H��tH�U H��tH9t<H��H���q�����u-H�M� H�5/� H�=�� �c���1�H��[]�f.�     �����H��H��tH�H��tH9t6H��������u*H�L� H�5ܱ H�=b� ����H��1�[]��    H��H������H��H��t=H�t$H���`���H��H��tPH�T$1�H;$�f���H���O�������H��[��]�H�� H�5b� H�=� ����H��1�[]�D  H�Ѭ H�5:� H�=�� �n���H��1�[]�D  SH�������H��tH�H��tH9t5H��H���*�����u&H�� H�5H� H�=n� ����1�[��     H������H��t3H�xH�� H��� H�5�� E1�1���� [H���W �    H��� H�5� H�=� ����1�[�f�UH��SH��H���/���H��tH�U H��tH9t<H��H���q�����u-H�M� H�5�� H�=�� �c���H��1�[]�f.�     H�������H��t;H�xH�5C� H�K� H��� E1�1�� � H��H��H��[]� �    H�� H�5J� H�=@� 