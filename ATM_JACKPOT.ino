#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// --- CONFIGURATION ---
const char* ssid = "Ploutus_Terminal"; // The Wi-Fi network your phone will connect to
const int servoPin = 13;               // GPIO pin connected to the servo signal wire

WebServer server(80);
Servo atmServo;

// This is the "Hacker Terminal" UI served to your phone
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Ploutus C2</title>
  <style>
    :root {
      --term-green: #39FF14; /* Camera-safe CRT green */
      --term-dark: #000000;
      --alert-red: #FF3333;  /* Camera-safe red */
    }
    body { background-color: var(--term-dark); color: var(--term-green); font-family: 'Courier New', Courier, monospace; margin: 0; padding: 20px; }
    .container { max-width: 800px; margin: 0 auto; border: 2px solid var(--term-green); padding: 30px; box-shadow: 0 0 15px var(--term-green); }
    h1 { text-align: center; border-bottom: 2px dashed var(--term-green); padding-bottom: 15px; font-size: 34px; font-weight: bold; }
    #terminal { min-height: 300px; margin-bottom: 20px; line-height: 1.8; font-size: 24px; font-weight: bold; }
    .cursor { display: inline-block; width: 14px; height: 24px; background-color: var(--term-green); animation: blink 1s step-end infinite; vertical-align: bottom; }
    @keyframes blink { 0%, 100% { opacity: 1; } 50% { opacity: 0; } }
    
    /* Updated Button Styling */
    button { 
      background-color: #111; 
      color: var(--term-green); 
      border: 3px solid var(--term-green); 
      padding: 20px; 
      font-size: 28px; 
      font-weight: bold; 
      font-family: inherit; 
      cursor: pointer; 
      width: 100%; 
      transition: 0.3s; 
      text-transform: uppercase; 
      letter-spacing: 2px; 
    }
    button:hover { background-color: var(--term-green); color: var(--term-dark); }
    button:disabled { border-color: #444; color: #444; background-color: #111; cursor: not-allowed; }
    
    .red-alert { color: var(--alert-red); font-weight: bold; text-shadow: 0 0 8px var(--alert-red); display: block; margin-top: 25px; font-size: 28px;}
  </style>
</head>
<body>
  <div class="container">
    <h1>TdA XFS Override v2.5</h1>
    <div id="terminal">
      <span id="output">Waiting for command...</span><span class="cursor"></span>
    </div>
    <button id="exploitBtn" onclick="runExploit()">[ INITIATE JACKPOT ]</button>
  </div>

  <script>
    const output = document.getElementById('output');
    const btn = document.getElementById('exploitBtn');

    const sequence = [
      "> Connecting to internal XFS bus... [OK]",
      "> Bypassing network authorization... [OK]",
      "> Locating Cash Dispensing Module... [FOUND]",
      "> Injecting Ploutus payload... [SUCCESS]",
      "> Sending dispense override command..."
    ];

    async function runExploit() {
      btn.disabled = true;
      output.innerHTML = "";
      
      for (let i = 0; i < sequence.length; i++) {
        output.innerHTML += sequence[i] + "<br>";
        await new Promise(r => setTimeout(r, 200)); 
      }

      try {
        let response = await fetch('/dispense');
        if(response.ok) {
          output.innerHTML += "<br><span style='color:#fff'>DISPENSE CONFIRMED.</span><br>";
          await new Promise(r => setTimeout(r, 400)); 
          output.innerHTML += "<span class='red-alert'>SYSTEM LOGS DELETED.<br>XFS LAYER RESET.</span>";
        } else {
          output.innerHTML += "<br><span style='color:var(--alert-red)'>ERROR: MODULE NOT RESPONDING</span>";
        }
      } catch(e) {
        output.innerHTML += "<br><span style='color:var(--alert-red)'>CONNECTION FAILED</span>";
      }
      
      setTimeout(() => { btn.disabled = false; output.innerHTML = "Waiting for next command..."; }, 4000);
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  
  // Configure the servo timers for the ESP32 architecture
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  atmServo.setPeriodHertz(50); 
  atmServo.attach(servoPin, 500, 2400); 

  // Set the servo to the starting position
  atmServo.write(0); 
  
  // Start the Wi-Fi Access Point
  Serial.println("\nStarting Ploutus Terminal...");
  WiFi.softAP(ssid); // No password required for the demo
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Connect to Wi-Fi 'Ploutus_Terminal' and go to IP: ");
  Serial.println(IP); // This will be 192.168.4.1

  // Handle the main webpage request
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", index_html);
  });

  // Handle the command to drop the cash
  server.on("/dispense", HTTP_GET, []() {
    Serial.println("Dispense command triggered!");
    
    // 1. Rotate the servo a full 180 degrees (all the way left)
    atmServo.write(180); 
    
    // 2. Wait 4 seconds to let the prop money slide all the way out
    delay(4000); 
    
    // 3. Reset the servo back to 0 degrees (all the way right) to close the trapdoor
    atmServo.write(0); 
    
    server.send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}