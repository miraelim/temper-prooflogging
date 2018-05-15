#!/bin/bash

file_primary_key_ctx=context.p_B1
file_signing_key_pub=opuB1_B8
file_signing_key_priv=oprB1_B8
file_signing_key_ctx=context_load_out_B1_B8
file_signing_key_name=name.load.B1_B8
file_input_data=secret.data
file_output_data=sig.4


handle_signing_key=0x81010005

alg_hash=0x000B
alg_primary_key=0x0001
alg_signing_key=0x0008

tpm2_verifysignature -c $file_signing_key_ctx -g 0x000B -m secret.data  -s sig.4 -t verify.txt                        
if [ $? != 0 ];then
exit 0
fi

./test
tpm2_sign -k 0x81010005 -g 0x000B -m secret.data  -s signoutput1.txt          
if [ $? != 0 ];then
exit 0
fi
