// $Id: CryptoPrimitives.h,v 1.1 2005/04/19 01:16:12 celaine Exp $

/*									tab:4
 * "Copyright (c) 2000-2003 The Regents of the University  of California.  
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Copyright (c) 2002-2003 Intel Corporation
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE     
 * file. If you do not find these files, copies can be found by writing to
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300, Berkeley, CA, 
 * 94704.  Attention:  Intel License Inquiry.
 */

/* Authors: Naveen Sastry
 * Date:    10/24/02
 */

/**
 * @author Naveen Sastry
 */


#define rol32(a, n) ( a = (((a) << (n)) | ((a) >> (32-(n)))))
#define ror32(a, n) ( a = (((a) >> (n)) | ((a) << (32-(n)))))
#define fastrol32 rol32
#define fastror32 ror32

// convert a 4byte char array to a unsigned long
// [assumes least significant byte of char array is first]
#define c2l(c,l)	(l =((unsigned long)(*((c)))),              \
			 l|=((unsigned long)(*((c+1))))<< 8L,       \
			 l|=((unsigned long)(*((c+2))))<<16L,       \
			 l|=((unsigned long)(*((c+3))))<<24L)
// convert an unsigned long to a 4 byte char array
// [assumes least significant byte of char array is first]
#define l2c(l,c)	(*((c))  =(unsigned char)(((l)     )&0xff), \
			 *((c+1))=(unsigned char)(((l)>> 8L)&0xff), \
			 *((c+2))=(unsigned char)(((l)>>16L)&0xff), \
			 *((c+3))=(unsigned char)(((l)>>24L)&0xff))

// convert a 2 byte char array to an unsigned short:
// [assumes MOST significant byte is first]
#define c2sM(c, s)       (s = ((unsigned short)(*((c))))  <<8L ,             \
                          s|= ((unsigned short)(*((c+1)))))

// convert a unsigned short to a 2 byte char array
// [assumes MOST significant byte is first]
#define s2cM(s, c)      (*((c))   = (unsigned short)(((s) >> 8L)&0xff), \
                         *((c+1)) = (unsigned short)(((s)      ) &0xff))

#define CRYPTO_TABLE_TYPE uint8_t
#define CRYPTO_TABLE_ACCESS(addr) (*(addr))
