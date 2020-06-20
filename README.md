# ESP-Roomlight

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

<h2>benötigte Bibliotheken</h2>

<table>
  <tr>
    <td>
      <b>Bibliotheksname</b>
    </td>
    <td>
      <b>Quelle</b>
    </td>
  </tr>
  
  <tr>
    <td>
      Arduino-LED Strip
    </td>
    <td>
      https://github.com/MadMax2506/Arduino-LED_Strip
    </td>
  </tr>
 
  <tr>
    <td>
      Arduino-Colors
    </td>
    <td>
      https://github.com/MadMax2506/Arduino-Colors
    </td>
  </tr>
  
  <tr>
    <td>
      Arduino-Storage
    </td>
    <td>
      https://github.com/MadMax2506/Arduino-Storage
    </td>
  </tr>
  
  <tr>
    <td>
      ESP-WLAN
    </td>
    <td>
      https://github.com/MadMax2506/ESP-Wlan
    </td>
  </tr>
  
  <tr>
    <td>
      ESP-Refresh over the air
    </td>
    <td>
      https://github.com/MadMax2506/ESP-Refresh_over_the_air
    </td>
  </tr>
  
  <tr>
    <td>
      ESP-MQTT
    </td>
    <td>
      https://github.com/MadMax2506/ESP-MQTT
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
          <td><code>color: r;g;b;w</code></td>
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

<h2>App fürs Smart-Home</h2>
http://localhost:8888/MadMax/ReactNative-SmartHome
