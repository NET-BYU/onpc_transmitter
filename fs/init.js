load('api_config.js');
load('api_dht.js');
load('api_mqtt.js');
load('api_timer.js');

let pin = Cfg.get('app.pin');
let sampleTime = Cfg.get('app.sample_time');
let id = Cfg.get('device.id');
let dataTopic = 'devices/' + id + '/data';
let metaTopic = 'devices/' + id + '/meta';
let statusTopic = 'devices/' + id + '/status';
let sequence = 0;

let dht = DHT.create(pin, DHT.DHT22);

// Things to do when devices boots up
Timer.set(10000 /* milliseconds */, false /* repeat */, function() {
  // Notify everyone the device is online
  let message = JSON.stringify({status: 'online'});
  let ok = MQTT.pub(statusTopic, message, 1, 1);
  print(message);
  print('Published:', ok ? 'yes' : 'no');

  // Update metadata
  message = JSON.stringify({temp: '°C', hum: '%', time: 's', seq: 'num'});
  ok = MQTT.pub(metaTopic, message, 1, 1);
  print(message);
  print('Published:', ok ? 'yes' : 'no');
}, null);


// Transmit data
Timer.set(sampleTime /* milliseconds */, true /* repeat */, function() {
  let t = dht.getTemp();
  let h = dht.getHumidity();

  if (isNaN(h) || isNaN(t)) {
    print('Failed to read data from sensor');
    return;
  }

  let message = JSON.stringify({temp: t, hum: h, time: Timer.now(), seq: sequence});
  let ok = MQTT.pub(dataTopic, message, 1, 0);

  print(message);
  print('Published:', ok ? 'yes' : 'no');

  sequence += 1;
}, null);
