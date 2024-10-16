import QtQuick 2.0

// Základem každého prvku je čtverec
Rectangle {
    id: btn;
    width: 100
    height: 100

    property alias text: txt.text;

    // Proměnné, které mají parametr property jsou přístupné zvenčí
    property color btnColor: "#777777";
    property color textColor: mouse.pressed ? "#0066FF" : "black";

    // Definování signálu
    signal clicked();

    border.color: "#bbbbbb";
    border.width: 3;

    // Change the button color conditionally
    color: mouse.pressed ? "lightgreen" : btnColor;


    // Samotná třída Rectangle nijak nezachytává signály
    // ze vstupních zařízení. Pro jejich zachytávání a zpracování
    // je potřeba vložit třídy k tomu určené.
    // Oblast zachytávající události myši
    // Lze použít oblushu události (onClicked, atd.)
    // uvnitř i mimo tento prvek, pak je zapotřebí použít
    // referenci přes id prvku. Lze i přistupovat k vlastnostem
    // objektu (vnitřní hodnoty), např. mouse.pressed (viz výše).
    MouseArea {
        id: mouse;
        anchors.fill: parent;
        onClicked: {
            btn.clicked();
        }
    }

    // Textový prvek pro zobrazení textu
    Text {
        id: txt;
        
        // Style the button text
        anchors.centerIn: parent;
        font.pointSize: 20;
    }

}
