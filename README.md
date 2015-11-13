# blatt2aufgabe2
Uni Projekt


Kommentar zum neuen Ordner und aktuellen Aufgaben:
"Scala wurde aufgegeben."
"Nein!"
"Doch!"
"Nein!"
"Doch!"
...
Ok, lassen wir das. Hier die kurze Erklärung:
Aktoren sind zu Highlevel. Zu viele Annahmen wurden schon getroffen, die wir nur noch mit einem dreckigen Hack umgehen können, wenn überhaupt. C++ ist zwar aufwendiger, bietet aber auch viel mehr Möglichkeiten.

Ich habe einfach mal ein Projekt erstellt, und die grundsätzlichen Sachen wie einer Server und ein (noch nicht benutzes) Message Format hinzugefügt.
Nachrichten können zur Zeit mittels:
	telnet localhost 1111
an den Server geschickt werden. Schickt man eine Nachricht die mit einem q anfängt, wird der Server beendet.

Was noch gebraucht wird:
	- Weiterleiten der Nachrichten an <localhost:_nextport>
	- synchronisierung

Was wir sicher irgendwann mal brauchen:
	- Serialisierung und Deserialisierung von Nachrichten

Die Steuerung über telnet finde ich gar nicht so schlimm, allerdings geht das sicher auch schöner.

simon

