#!/bin/bash
file_primary_key_ctx=keyprimary.txt
file_signing_key_pub=pub.txt
file_signing_key_priv=priv.txt
file_signing_key_ctx=signkey.txt
file_signing_key_name=name.txt

file_input_data_hash=secret_hash.txt
file_input_data_hash_tk=secret_hash_tk.txt

handle_signing_key=0x81010005

alg_hash=0x000B
alg_primary_key=0x0001
alg_signing_key=0x0001
nv_test_index=0x1500018
nv_auth_handle=0x40000001


beginTime=$(date +%s%N)
    rm name.txt pub.txt priv.txt verify.txt keyoutput.txt

takeownershipbeginTime=$(date +%s%N)
    tpm2_takeownership -c
takeownershipendTime=$(date +%s%N)
    takeownershipelapsed=`echo "($takeownershipendTime - $takeownershipbeginTime) / 1000000" | bc`
    takeownershipelapsedSec=`echo "scale=6;$takeownershipelapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo TAKEOWNERSHIP: $takeownershipelapsedSec sec >> time.txt


createprimarybeginTime=$(date +%s%N)
    tpm2_createprimary -A e -g $alg_hash -G $alg_primary_key -C $file_primary_key_ctx
    if [ $? != 0 ];then
    fail createprimary
    fi
createprimaryendTime=$(date +%s%N)
    createprimaryelapsed=`echo "($createprimaryendTime - $createprimarybeginTime) / 1000000" | bc`
    createprimaryelapsedSec=`echo "scale=6;$createprimaryelapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo CREATAEPRIMARY: $createprimaryelapsedSec sec >> time.txt

createbeginTime=$(date +%s%N)
    tpm2_create -g $alg_hash -G $alg_signing_key -o $file_signing_key_pub -O $file_signing_key_priv  -c $file_primary_key_ctx
    if [ $? != 0 ];then
    fail create
    fi
createendTime=$(date +%s%N)
    createelapsed=`echo "($createendTime - $createbeginTime) / 1000000" | bc`
    createelapsedSec=`echo "scale=6;$createelapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo CREATE: $createelapsedSec sec >> time.txt



loadbeginTime=$(date +%s%N)
    tpm2_load -c $file_primary_key_ctx  -u $file_signing_key_pub  -r $file_signing_key_priv     -n $file_signing_key_name -C $file_signing_key_ctx
    if [ $? != 0 ];then
    fail load
    fi
loadendTime=$(date +%s%N)
    loadelapsed=`echo "($loadendTime - $loadbeginTime) / 1000000" | bc`
    loadelapsedSec=`echo "scale=6;$loadelapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo LOAD: $loadelapsedSec sec >> time.txt


nvlist1beginTime=$(date +%s%N)
    tpm2_nvlist|grep -i $nv_test_index
    if [ $? = 0 ];then
nvlist1endTime=$(date +%s%N)
    nvlist1elapsed=`echo "($nvlist1endTime - $nvlist1beginTime) / 1000000" | bc`
    nvlist1elapsedSec=`echo "scale=6;$nvlist1elapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo NVLIST1: $nvlist1elapsedSec sec >> time.txt


nvrelease1beginTime=$(date +%s%N)

    tpm2_nvrelease -x $nv_test_index -a $nv_auth_handle
    if [ $? != 0 ];then
    echo "please release the nv index $nv_test_index first!"
    exit 1
    fi
nvrelease1endTime=$(date +%s%N)
    nvrelease1elapsed=`echo "($nvrelease1endTime - $nvrelease1beginTime) / 1000000" | bc`
    nvrelease1elapsedSec=`echo "scale=6;$nvrelease1elapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo NVRELEASE1: $nvrelease1elapsedSec sec >> time.txt

    fi

nvdefine1beginTime=$(date +%s%N)
    tpm2_nvdefine -x $nv_test_index -a $nv_auth_handle -s 32 -t 0x2000A
    if [ $? != 0 ];then
    echo "nvdefine fail,Please check your environment!"
    exit 1
    fi
nvdefine1endTime=$(date +%s%N)
    nvdefine1elapsed=`echo "($nvdefine1endTime - $nvdefine1beginTime) / 1000000" | bc`
    nvdefine1elapsedSec=`echo "scale=6;$nvdefine1elapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo NVDEFINE1: $nvdefine1elapsedSec sec >> time.txt


test1beginTime=$(date +%s%N)
    ./test1

test1endTime=$(date +%s%N)
    test1elapsed=`echo "($test1endTime - $test1beginTime) / 1000000" | bc`
    test1elapsedSec=`echo "scale=6;$test1elapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo TEST1: $test1elapsedSec sec >> time.txt

sign1beginTime=$(date +%s%N)

    tpm2_sign -c $file_signing_key_ctx  -g $alg_hash -m key.txt -s keyoutput.txt

sign1endTime=$(date +%s%N)
    sign1elapsed=`echo "($sign1endTime - $sign1beginTime) / 1000000" | bc`
    sign1elapsedSec=`echo "scale=6;$sign1elapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo SIGN1: $sign1elapsedSec sec >> time.txt


nvwrite1beginTime=$(date +%s%N)
    tpm2_nvwrite -x $nv_test_index -a $nv_auth_handle  -f counter.txt
    if [ $? != 0 ];then
    echo "nvwrite fail!"
    exit 1
    fi
nvwrite1endTime=$(date +%s%N)
    nvwrite1elapsed=`echo "($nvwrite1endTime - $nvwrite1beginTime) / 1000000" | bc`
    nvwrite1elapsedSec=`echo "scale=6;$nvwrite1elapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo NVWRITE1: $nvwrite1elapsedSec sec >> time.txt



verifybeginTime=$(date +%s%N)
    tpm2_verifysignature -c $file_signing_key_ctx -g 0x000B -m key.txt  -s keyoutput.txt -t verify.txt
    if [ $? != 0 ];then
    exit 0
    fi
verifyendTime=$(date +%s%N)
    verifyelapsed=`echo "($verifyendTime - $verifybeginTime) / 1000000" | bc`
    verifyelapsedSec=`echo "scale=6;$verifyelapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo VerifyTOTAL: $verifyelapsedSec sec >> time.txt


counter=$(cat counter.txt)

#    tpm2_nvlist|grep -i $nv_test_index
#   if [ $? = 0 ];then
#  tpm2_nvrelease -x $nv_test_index -a $nv_auth_handle
#   if [ $? != 0 ];then
#   echo "please release the nv index $nv_test_index first!"
#    exit 1
#   fi
#   fi


#   tpm2_nvdefine -x $nv_test_index -a $nv_auth_handle -s 32 -t 0x2000A
#   if [ $? != 0 ];then
#   echo "nvdefine fail,Please check your environment!"
#   exit 1
#   fi


nvread1beginTime=$(date +%s%N)
    tpm2_nvread -x $nv_test_index -a $nv_auth_handle  -s 64 -o 0

    if [ $? != 0 ];then
    echo "nvread fail!"
    exit 1
    fi

nvread1endTime=$(date +%s%N)
    nvread1elapsed=`echo "($nvread1endTime - $nvread1beginTime) / 1000000" | bc`
    nvread1elapsedSec=`echo "scale=6;$nvread1elapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo NVREAD1: $nvread1elapsedSec sec >> time.txt




test2beginTime=$(date +%s%N)
    ./test2
test2endTime=$(date +%s%N)
    test2elapsed=`echo "($test2endTime - $test2beginTime) / 1000000" | bc`
    test2elapsedSec=`echo "scale=6;$test2elapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo TEST2: $test2elapsedSec sec >> time.txt


#    tpm2_nvwrite -x $nv_test_index -a $nv_auth_handle  -f counter.txt
#   if [ $? != 0 ];then
#   echo "nvwrite fail!"
#   exit 1
#   fi

    rm keyoutput.txt

sign2beginTime=$(date +%s%N)

    tpm2_sign -c $file_signing_key_ctx  -g 0x000B -m key.txt -s keyoutput.txt
    if [ $? != 0 ];then
    echo "sign fail"
    exit 0
    fi
sign2endTime=$(date +%s%N)
    sign2elapsed=`echo "($sign2endTime - $sign2beginTime) / 1000000" | bc`
    sign2elapsedSec=`echo "scale=6;$sign2elapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo sign2: $sign2elapsedSec sec >> time.txt


endTime=$(date +%s%N)
    elapsed=`echo "($endTime - $beginTime) / 1000000" | bc`
    elapsedSec=`echo "scale=6;$elapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo TOTAL: $elapsedSec sec >> time.txt


#beginTime=$(date +%s%N)
#   tpm2_verifysignature -c $file_signing_key_ctx -g 0x000B -m secret.data  -s sig.4 -t verify.txt                        
#   if [ $? != 0 ];then
#   exit 0
#   fi
#endTime=$(date +%s%N) 
#    elapsed=`echo "($endTime - $beginTime) / 1000000" | bc` 
#    elapsedSec=`echo "scale=6;$elapsed / 1000" | bc | awk '{printf "%.6f", $1}'` 
#   echo TOTAL: $elapsedSec sec >> time.txt

#./test
#    tpm2_sign -k 0x81010005 -g 0x000B -m secret.data  -s signoutput1.txt          
#   if [ $?$? != 0 ];then
#   exit 0
#   fi
