#! /bin/bash/

read -p "username: " username
echo "password: "
stty -echo
 read password
stty echo

timedate() {
	date +"%D %T"
}

ts=$(timedate)

verifyunamepass() {
	true=$(awk -v username="${username}" -v password="${password}" '$1==username && $2==password {print 1}' ./users/user.txt)

	until [[ "$true" == 1 ]];
	do
		echo "$ts LOGIN: ERROR Failed login attempt on user $username" >> log.txt
		read -p 'Username: ' username
		echo "Password: "
		stty -echo
			read password
		stty echo
		true=$(awk -v username="${username}" -v password="${password}" '$1==username && $2==password {print 1}' ./users/user.txt)
	done
}

downloadpict() {
	sudo apt install zip unzip
	folder=`date +%F`"_$username"
	index=0
	if [[ ! -f "./$folder.zip" ]]
	then
		mkdir -p "./$folder"
	else
		unzip -P "$password" "./$folder.zip"
		file=`ls "$folder" -r | head -n 1`
		index=$(awk -F"_" '{print $2}' <<< "$file")
	fi
	for ((num=index+1; num<=index+q; num++))
	do
		wget -O "$folder/PIC_$num" https://loremflickr.com/320/240
	done
	zip -r -P "$password" "$folder.zip" "$folder"
	rm -rf "$folder"
}

loginattempt() {
	awk -v username="${username}" '$3=="LOGIN:" && ($6==username || $10==username) {++n} END {print "login attempt = " n}' log.txt
}

verifyunamepass

echo "$ts LOGIN: INFO User $username logged in" >> log.txt

read -p 'Type Command: ' com
cmd=$(awk '{print $1}' <<< "$com")
q=$(awk '{print $2}' <<< "$com")

if [[ "$cmd" == "dl" ]]
then
	downloadpict
elif [[ "$cmd" == "att" ]]
then
	loginattempt
fi
