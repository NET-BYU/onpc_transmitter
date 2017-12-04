load('api_config.js');
load('api_gpio.js');
load('api_mqtt.js');
load('api_net.js');
load('api_sys.js');
load('api_timer.js');
load('api_queue.js');

let led = Cfg.get('pins.led');
let getRssi = ffi('int wifi_station_get_rssi(void)');
let topic = '/device/x/data';

let sequence = 0;


let getInfo = function() {
  return JSON.stringify({
    total_ram: Sys.total_ram(),
    free_ram: Sys.free_ram()
  });
};

// Blink built-in LED every second
GPIO.set_mode(led, GPIO.MODE_OUTPUT);
Timer.set(1000 /* 1 sec */, true /* repeat */, function() {
  let value = GPIO.toggle(led);
  print(value ? 'Tick' : 'Tock', 'uptime:', Sys.uptime(), getInfo(), getRssi());
}, null);

let minDelay = 500;
let maxDelay = 5000;

function upload_data(delay) {
  Timer.set(delay, false, function() {
    print("%%%%%%%%%%%%%%%%%%%%%%%%%% Peeking at data");
    Queue.peek(function(data) {
      print("Data peeked:", data);

      if (!data) {
        print("%%%%%%%%%%%%%%%%%%%%%%%%%% No data left, trying again later");
        upload_data(maxDelay);
        return;
      }

      // Valid data -- let's publish it
      print("%%%%%%%%%%%%%%%%%%%%%%%%%% Trying to upload data");
      let ok = MQTT.pub(topic, data, 1);

      if (ok) {
        print("%%%%%%%%%%%%%%%%%%%%%%%%%% Deleting data");
        Queue.remove();
        // Try getting more data
        upload_data(minDelay);
      }
      else {
        // Try again later
        print("%%%%%%%%%%%%%%%%%%%%%%%%%% Unable to upload data -- try again later");
        upload_data(maxDelay);
      }
      print("%%%%%%%%%%%%%%%%%%%%%%%%%% Done");
    }, null);
  }, null);
}


Timer.set(2000 /* 2 sec */, false /* repeat */, function() {
  print("######################### Starting queue");
  let startResult = Queue.start();
  print("Result: ", startResult);

  if (!startResult) {
    print("Unable to start queue!");
    return;
  }

  // "Generate" data
  Timer.set(2000 /* 2 sec */, true /* repeat */, function() {
    print("######################### Publishing data");
    let data = JSON.stringify({
      pm25: 100,
      pm10: 50,
      rssi: getRssi(),
      sequence: sequence,
      time: Timer.now()
    });
    print(data);
    let ok = MQTT.pub(topic, data, 1);

    if (!ok) {
      print("######################### Unable to publish data");
      print("######################### Adding data to queue");
      let result = Queue.put(data);
      print("Put result:", result);
    }

    sequence += 1;
  }, null);

  print("######################### Starting process to upload data");
  upload_data(maxDelay);
}, null);


// Monitor network connectivity.
Net.setStatusEventHandler(function(ev, arg) {
  let evs = '???';
  if (ev === Net.STATUS_DISCONNECTED) {
    evs = 'DISCONNECTED';
  } else if (ev === Net.STATUS_CONNECTING) {
    evs = 'CONNECTING';
  } else if (ev === Net.STATUS_CONNECTED) {
    evs = 'CONNECTED';
  } else if (ev === Net.STATUS_GOT_IP) {
    evs = 'GOT_IP';
  }
  print('== Net event:', ev, evs);
}, null);
