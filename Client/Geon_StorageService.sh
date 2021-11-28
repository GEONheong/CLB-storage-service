#!/bin/bash

while :
do
    echo "1.Sign-In to Gstorage"
    echo "2.Sign-up to Gstorage"
    echo -n "Enter :"
    read menuNum

    if [ $menuNum -eq 1 ];then
        echo -n "Input service ID : "
        read ID

        echo -n "Input service Password : "
        stty -echo
        read PASSWORD
        stty echo

        clear

        ./signService.d/signInService $ID $PASSWORD
        break
    elif [ $menuNum -eq 2 ];then
        echo -n "Create ID : "
        read ID

        echo -n "Create Password : "
        stty -echo
        read PASSWORD
        stty echo

        clear

        ./signService.d/signUpService $ID $PASSWORD
        break
    else
        clear
        echo "--------------------------------------\n"
		echo "Try again...(Wrong Input or No option)\n"
		echo "--------------------------------------\n"
    fi
done

