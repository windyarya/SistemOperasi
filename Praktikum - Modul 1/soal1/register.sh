#! /bin/bash/

mkdir -p ./users
test -f ./users/user.txt || touch ./users/user.txt
test -f log.txt || touch log.txt

read -p 'username: ' username

timedate() {
	date +"%D %T"
}

ts=$(timedate)

checkuname() {
	until ! grep -q "$username" ./users/user.txt;
do
	echo "username already exists"
	echo "$ts REGISTER: ERROR user already exists" >> log.txt
	read -p 'new username: ' username
done
}

checkpass() {
	until [[ ! "$password" == "$username" && ${#password} -ge 8 && "$password" == *[A-Z]* && "$password" == *[a-z]* && "$password" == *[1-9]* ]];
	do
		echo "Password Tidak Sesuai Kriteria!"
		echo "password: " 
		stty -echo 
			read password
		stty echo
	done
}

checkuname

echo "password " 
stty -echo 
        read password 
stty echo  

checkpass

echo "REGISTER: INFO User $username registered successfully" >> log.txt
echo "$username $password" >> ./users/user.txt
