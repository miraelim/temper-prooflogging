#;**********************************************************************;
#
# Copyright (c) 2016, Intel Corporation
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, 
# this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice, 
# this list of conditions and the following disclaimer in the documentation 
# and/or other materials provided with the distribution.
#
# 3. Neither the name of Intel Corporation nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
# THE POSSIBILITY OF SUCH DAMAGE.
#;**********************************************************************;
#!/bin/bash
file_primary_key_ctx=context.p_B1
file_signing_key_pub=opuB1_B8
file_signing_key_priv=oprB1_B8
file_signing_key_ctx=context_load_out_B1_B8
file_signing_key_name=name.load.B1_B8
file_input_data=key.txt
file_output_data=keyobject.txt
file_verify_tk_data=ticket.txt 

file_input_data_hash=secret_hash.data
file_input_data_hash_tk=secret_hash_tk.data

handle_signing_key=0x81010005

alg_hash=0x000B
alg_primary_key=0x0001
alg_signing_key=0x0001

rm $file_verify_tk_data
fail()
{
    echo "$1 test fail, please check the environment or parameters!"
	exit 1
}
Pass()
{
    echo ""$1" pass" >>test_getpubak_pass.log
}

tpm2_verifysignature -c $file_signing_key_ctx  -g $alg_hash -m $file_input_data  -s $file_output_data -t $file_verify_tk_data
if [ $? != 0 ];then
fail verifysignature
exit 1

else 
echo "verify success"
fi

