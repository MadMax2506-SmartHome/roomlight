# ESP-Roomlight

## Allgemein
* Gerätename: `roomlight`
* benötigt einen externen MQTT-Server (IP-Adresse und Port können im Quellcode angepasst werden)
* benötigt einen WLan-Zugriff (SSID und Passwort können im Quellcode angepasst werden)

<h2>Verkabelung</h2>

<table>
    <tr>
        <td>rot</td>
        <td>5V Spannung</td>
    </tr>
    <tr>
        <td>gelb</td>
        <td>Erde</td>
    </tr>
    <tr>
        <td>weiß</td>
        <td>
            <table>
                <tr>
                    <td>Data - Wandseite vorne</td>
                    <td>60x LEDs</td>
                    <td>2 Knoten im Kabel</td>
                </tr>
                <tr>
                    <td>Data - Wandseite hinten</td>
                    <td>60x LEDs</td>
                    <td>1 Knoten im Kabel</td>
                </tr>
                <tr>
                    <td>Data - Seite</td>
                    <td>50x LEDs</td>
                    <td>-</td>
                </tr>
            </table>
        </td>
    </tr>
</table>

<h2>MQTT</h2>

<table>
  <tr>
    <td>
      <b>Topic</b>
    </td>
    <td>
      <b>Beschreibung</b>
    </td>
  </tr>

  <tr>
    <td>
      <code>devices</code>
    </td>
    <td>
      Smart-Home Geräte
      <table>
        <tr>
          <td><code>list-devices</code></td>
          <td>            
            Listet alle Geräte mit folgenden Informationen auf (JSON-Format)
            <ul>
              <li>Name</li>
              <li>Mac-Adresse</li>
              <li>Status-Topics</li>
              <li>Konfigurations-Topics</li>
            </ul>
          </td>
        </tr>
      </table>
    </td>
  </tr>

  <tr>
    <td><code>status/<i>*mac*</i></code></td>
    <td>
      Anzeige der aktuellen Einrichtung des Roomlights für jede Unterteilung (JSON-Format)
      <ul>
        <li>Status</li>
        <li>Animationsfarbe (RGB)</li>
        <li>Animationsrichtung</li>
        <li>Animationsart</li>
        <li>Animationszeit</li>
      </ul>
    </td>
  </tr>

  <tr>
    <td>
        <ul>
            <li><code>status/<i>*mac*</i>/keyboard</code></li>
            <li><code>status/<i>*mac*</i>/bed/wall</code></li>
            <li><code>status/<i>*mac*</i>/bed/side</code></li>
        </ul>
    </td>
    <td>
      spezifizierter Status für jede Unterteilungen des Roomlights
      <ul>
        <li><code>power-on</code></li>
        <li><code>active</code></li>
        <li><code>idle</code></li>
      </ul>
    </td>
  </tr>

  <tr>
    <td><code>conf/<i>*mac*</i></code></td>
    <td>
      globale Konfiguration für alle Unterteilungen des Roomlights
      <table>
        <tr>
          <td><code>get-conf</code></td>
          <td>
            aktuelle Einrichtung des Roomlights anzeigen in: &nbsp<code>status/<i>*mac*</i></code>
          </td>
        </tr>
      </table>
    </td>
  </tr>

  <tr>
    <td>
         <ul>
            <li><code>conf/<i>*mac*</i>/keyboard</code></li>
            <li><code>conf/<i>*mac*</i>/bed/wall</code></li>
            <li><code>conf/<i>*mac*</i>/bed/side</code></li>
        </ul>
    </td>
    <td>
      spezifizierte Konfiguration für jede Unterteilungen des Roomlights
      <table>
        <tr>
          <td><code>color: r;g;b</code></td>
          <td>Animationsfarbe <i>(abgesehen von der Rainbow-Animation)</i></td>
        </tr>
        <tr>
          <td><code>orientation: c</code></td>
          <td>
            Animationsrichtung
            <ul>
              <li>von rechts nach links <code>'l'</code></li>
              <li>von links nach rechts <code>'r'</code></li>
              <li>von der Mitte ausgehend <code>'c'</code> <br> <i>(abgesehen von der Rainbow-Animation)</i></li>
            </ul>
          </td>
        </tr>
        <tr>
          <td>
            <code>animation-typ: str</code>
          </td>
          <td>
            Animationsart
            <ul>
              <li><code>"fade"</code></li>
              <li><code>"rainbow"</code></li>
              <li><code>"toColor"</code> <i></i></li>
            </ul>
          </td>
        </tr>
        <tr>
          <td>
            <code>animation-time: t</code>
          </td>
          <td>
            Zeit für die Animation pro LED
          </td>
        </tr>
        <tr>
          <td>
            <code>restart-animation</code>
          </td>
          <td>
            Neustart der Animation
          </td>
         </tr>
         <tr>
          <td>
            <code>active</code>
           </td>
          <td>
            Streifen in den Zustand aktiv setzen
           </td>
        </tr>
         <tr>
          <td>
            <code>idle</code>
           </td>
          <td>
            Streifen in den Zustand Leerlauf setzen
           </td>
        </tr>
         <tr>
          <td>
            <code>status</code>
           </td>
          <td>
            Zustand des Streifens ins Status-Topic schreiben
           </td>
        </tr>
        <tr>
          <td>
            <code>reload-conf</code>
          </td>
          <td>
            Wiederherstellung der Standardkonfiguration
          </td>
        </tr>
        <tr>
          <td>
            <code>save-conf</code>
          </td>
          <td>
            Speichert die Konfiguration
          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>

<h2>Setup</h2>

<h3>ESP32 einbinden</h3>
<ol>
	<li>Arduino IDE  öffnen</li>
	<li>Voreinstellungen öffnen: Datei > Voreinstellungen</li>
	<li>
		folgende Links zu Zusätzliche Boardverwalter URLs hinzufügen:
        <ul>
	        <li>https://dl.espressif.com/dl/package_esp32_index.json</li>
			<li>https://arduino.esp8266.com/stable/package_esp8266com_index.json</li>
		</ul>
     </li>
    <li>Voreinstellungen schließen</li>
    <li>Boardverwalter  öffnen: Werkzeuge > Board > Boardverwalter</li>
    <li>Boards von <b>esp8266 (by ESP8266 Community)</b> installieren -> <b>zu suchen: "esp8266"</b></li>
    <li>Boardverwalter schließen </li>
    <li>Board auswählen: Werkzeuge > Board > <b>LOLIN (Wemos) D1 R2 & mini </b></li>
</ol>

<h3>Bibliotheken einbinden</h3>
<ol>
	<li>Arduino IDE  öffnen</li>
    <li>Bibliothekverwalter  öffnen: Werkzeuge > Bibliothek verwalten  </li>
    <li>
	    folgende Bibliotheken installieren:
        <ul>
	        <li>ArduinoOTA (by Arduino, Juraj Andrassy) -> <b>zu suchen: "ota"</b></li>
	        <li>Adafruit NeoPixel (by Adafruit ) -> <b>zu suchen: "neo pixel"</b></li>
	        <li>PubSublient (by Nick O'Leary) -> <b>zu suchen: "pubsub client"</b></li>
        </ul>
		</li>
</ol>

<h3>Refresh over the air</h3>
<ol>
	<li>Arduino IDE öffnen</li>
    <li>Netzwerkschnittstelle auswählen: Werkzeuge > Port > <i>*Mac-Adresse*</i></li>
    <li>Code hochladen</li>
</ol>
