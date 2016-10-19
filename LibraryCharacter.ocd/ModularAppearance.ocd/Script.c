/**
 This is the basic functionality for displaying an object with additional/optional parts,
 such as accessories or clothing.
 
 @title Modular Appearance
 @author Marky
 @version 0.1.0
 */
 
// Was sollte das Zeug haben?
// - Slots für die einzelnen Teile
// - man kann die Slots mit einer Fallback-Grafik belegen, die angezeigt wird, wenn man nichts benutzt
// - ein Slot zeigt entweder die aktuelle Grafik an, oder die Fallback-Grafik, falls vorhanden, oder nichts
// - die aktuelle Grafik / Attachment-ID des Meshes sollte in einem Array gespeichert sein
// - evtl. sollte das attachde Objekt im Array enthalten sein
// - wenn ein Objekt attached wird, dann sollte die Mesh-Grafik in diesem Objekt abgefragt werden? Oder das Objekt fragt beim Ziel an, ob es einen Skin für diese Grafik hat. Wahrscheinlich ist letzteres besser. Ansonsten eben die Defaul-Grafik
// - Priorität ist explizit nicht notwendig, es kann einfach nur ein Ding je Slot verwendet werden.
// - wahrscheinlich sollte in dem Array jeweils eine Proplist enthalten sein mit {definition, attachmentID, graphicsName usw?} 
 