#! /bin/bash/

user=$(whoami)
mkdir -p /home/"$user"/log

file="metrics_"`date +%Y%m%d%H%M%S`".log"

metrics() {
	mem=$(awk '$1=="Mem:" {printf "%s,%s,%s,%s,%s,%s", $2, $3, $4, $5, $6, $7}' <<< `free -m`)
	swap=$(awk '$1=="Swap:" {printf "%s,%s,%s", $2, $3, $4}' <<< `free -m`)
	dir=$(awk '{printf "%s", $1}' <<< `du -sh /home/"$user"`)

	echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" 
	echo "$mem,$swap,/home/$user/,$dir"
}

metrics >> /home/"$user"/log/"$file"
chown "$user" "/home/"$user"/log/"$file""
