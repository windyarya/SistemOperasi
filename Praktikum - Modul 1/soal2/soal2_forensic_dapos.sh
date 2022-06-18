#! /bin/bash/

mkdir -p ./forensic_log_website_daffainfo_log
test -f ./forensic_log_website_daffainfo_log/ratarata.txt || touch ./forensic_log_website_daffainfo_log/ratarata.txt
test -f ./forensic_log_website_daffainfo_log/result.txt || touch ./forensic_log_website_daffainfo_log/result.txt

#rata-rata kunjungan
total=$(awk 'END{print NR-1}' log_website_daffainfo.log)
echo "Rata-rata serangan adalah sebanyak n requests per jam" >> ./forensic_log_website_daffainfo_log/ratarata.txt

#ip address terbanyak
awk -F'"' '{ipc[$2]++;} END {max=0; for (IP in ipc) {if (ipc[IP] > max) {max = ipc[IP]; ipaddr = IP;}} printf("IP yang paling banyak mengakses server adalah: %s sebanyak %d requests\n", ipaddr, max)}' log_website_daffainfo.log >> ./forensic_log_website_daffainfo_log/result.txt

#jumlah curl
tcurl=$(awk 'BEGIN {count=0} $NF ~ /curl/ {count++} END {print count}' log_website_daffainfo.log)
echo "Ada $tcurl requests yang menggunakan curl sebagai user-agent" >> ./forensic_log_website_daffainfo_log/result.txt

#ip jam 2 pagi
for i in total
do
	ipaddr=$(awk -F '[" : ]' '$6==02 {print $2}' log_website_daffainfo.log)
	echo "$ipaddr" >> ./forensic_log_website_daffainfo_log/result.txt
done

