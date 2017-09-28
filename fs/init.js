load('api_config.js');
load('api_dht.js');
load('api_timer.js');

let pin = Cfg.get('app.pin');
let dht = DHT.create(pin, DHT.DHT22);
let sampleTime = Cfg.get('app.sample_time')

Timer.set(sampleTime /* milliseconds */, true /* repeat */, function() {
  let t = dht.getTemp();
  let h = dht.getHumidity();

  if (isNaN(h) || isNaN(t)) {
    print('Failed to read data from sensor');
    return;
  }

  print('Temperature:', t, '*C');
  print('Humidity:', h, '%');
}, null);
