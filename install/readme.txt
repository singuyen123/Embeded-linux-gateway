
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
while :
do
  /home/pi/workPlace/Embeded-linux-gateway/src/service_user_space/client_SPI_Lo$
 # sleep 1
#  /home/pi/workPlace/Embeded-linux-gateway/src/service_user_space/clientUART/c$
  sleep 5m
  sudo kill -TERM  $(ps aux | grep lora_RF_client | awk '{print $2}')
  #sleep 5
  #sudo kill -TERM $(ps aux | grep clientUART | awk '{print $2}')
  sleep 5
done


//////////////////////////////////////////////////////////////////
home/pi/workPlace/Embeded-linux-gateway/src/service_user_space/clientUART/clientUART $
/////////////////////////////////////////////

[Unit]
Description= Service Lora_RF
After=multi-user.target

[Service]
#Type=forking
ExecStart=/bin/bash /usr/local/sbin/startup_app.sh

[Install]
WantedBy=multi-user.target

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
 [Unit]
 Description=uartClient Service
 After=multi-user.target

 [Service]
 Type=forking
 ExecStart=/bin/bash /usr/local/sbin/startup_app_bluetooth.sh

 [Install]
 WantedBy=multi-user.target
 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
console=serial0,115200 console=tty1