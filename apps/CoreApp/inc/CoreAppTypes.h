/*
Copyright (c) 2018, Globallogic s.r.o.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of the Globallogic s.r.o. nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC S.R.O. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* 
 * File:   CoreAppType.h
 * Author: Peter Kocity
 *
 * Created on February 7, 2018, 2:31 PM
 */



#ifndef COREAPPTYPE_H
#define COREAPPTYPE_H

#include <iostream>


namespace flexd {
    namespace core {
        
        namespace RqstType {

            enum Enum {
                install = 0x00,
                unintall,
                start,
                stop,
                freez,
                unfreez,
                update,
                undefined = 0xFF
            };
        }

        namespace RqstAck {

            enum Enum {
                succes = 0x00,
                fail,
                undefined = 0xFF,
            };
        }

        namespace JobState {

            enum Enum {
                jobNotExist = 0x00,
                running,
                peding,
                busy,
                /*...*/
                undefined = 0xFF
            };
        }
        
        
    } // namespace core
} // namespace flexd

#endif /* COREAPPTYPE_H */

