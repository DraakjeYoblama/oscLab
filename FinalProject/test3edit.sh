port=5678
clients=3
sleep 2
echo -e 'starting 3 sensor nodes'
./sensor_node 15 3 127.0.0.1 $port &
sleep 2
./sensor_node 37 1 127.0.0.1 $port &
sleep 2
./sensor_node 21 2 127.0.0.1 $port &
sleep 20
killall sensor_node
sleep 30
killall sensor_gateway
