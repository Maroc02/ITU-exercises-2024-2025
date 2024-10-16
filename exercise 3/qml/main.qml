import QtQuick 2.1
import QtQuick.Window 2.0
import "../js/Theme.js" as Theme
import cz.vutbr.fit 1.0

/**
 * Author: Marek Čupr
 * Description: Simple calculator build with QML.
 * Date: 16.10.2024
*/

Window {
    visible: true
    width: 400
    height: 315 + display.height;
    
    title: "ITU - Qt 5 / QML kalkulačka"

    // Definování datového modelu s operátory
    // 'op' - zkratka pro operaci
    // 'tog' - zkratka pro toggled, označení, která operace je vybrána
    ListModel {
        id: operations;
        ListElement { op: "+"; tog: false; }
        ListElement { op: "-"; tog: true; }
        // Add the operators to the screen
        ListElement { op: "*"; tog: false; }
        ListElement { op: "/"; tog: false; }
    }

    // Prvek pro rozvržení prvků do sloupce
    // http://en.wikipedia.org/wiki/Layout_%28computing%29
    // https://qmlbook.github.io/ch04-qmlstart/qmlstart.html#positioning-elements
    Column {

        // Vstupní hodnota - první operand výpočtu
        Rectangle {
            id: input;
            height: 35;
            width: 400;
            border.color: "#bbb";
            border.width: 3;
            anchors.margins: 2;
            // Change the input background color if error occures
            color: errorOccured ? "red" : "#777";

            // Boolean indicating if error occured
            property bool errorOccured;

            TextInput {
                anchors.fill: parent;
                anchors.margins: 2;
                horizontalAlignment: TextInput.AlignLeft;
                verticalAlignment: TextInput.AlignVCenter;
                id: textA;
                font.pointSize: 22;
                text: "0";
            }
        }

        // Prvek pro rozvržení prvků do řádku
        // Více jak prvek Column (výše)
        Row {
            // Obdoba ListView (ale více obecný) nebo funkce foreach()
            // obsahuje _model_ a _delegate_
            Repeater {
                // Definování modelu, data pro zobrazení
                model: operations;

                // Delegování vzhledu v MVC
                // Jak má vypadat jeden prvek
                // @disable-check M301
                delegate: MyButton {
                    btnColor: Theme.btn_colour
                    
                    text: model.op
                    toggled: model.tog;
                    
                    onClicked: {
                        for (var i = 0; i < operations.count; i++) {
                            operations.setProperty( i, "tog", (i == index) );
                        }
                    }
                }
            }

        }

        // "Vlastní" třída pro posuvník. Definice v MySlider.qml
        // @disable-check M301
        MySlider {
            id: slider
            color: Qt.darker(Theme.slider_color)
            // Change the slider background color if error occures
            rectColor: errorOccured ? "red" : Theme.slider_color;

            // Boolean indicating if error occured
            property bool errorOccured;
        }

        // Rectangle to display the slider value
        Rectangle {
            id: display;
            height: 35;
            width: 400;
            border.color: "#bbb";
            border.width: 3;
            anchors.margins: 2;
            color: "#777";
            Text {
                anchors.fill: parent;
                anchors.margins: 2;
                text: lut.getValue(slider.value); // Get the sider value
                font.pointSize: 15;
            }
        }

        // Vlastní klikací tlačítko. Definice v MyClickButton.qml
        // @disable-check M301
        MyClickButton {
            width: 400;
            btnColor: Theme.btn_colour
            
            text: qsTr( "Compute" )
            
            function getOperation() {
                for (var i = 0; i < operations.count; i++) {
                    var item = operations.get(i);
                    if (item.tog) {
                        return item.op;
                    }
                }
                return "+";
            }

            function handle_input_error()
            {
                input.errorOccured = true;
                displayValue.text = "ERROR::Value is not a number";
                displayValue.horizontalAlignment = Text.AlignHCenter;
            }

            function handle_slider_error()
            {
                slider.errorOccured = true;
                displayValue.text = "ERROR::Zero division";
                displayValue.horizontalAlignment = Text.AlignHCenter;
            }

            // Obsluha události při zachycení signálu clicked
            onClicked: {
                var a = parseFloat(textA.text, 10);
                // Check if the input value is a number
                if (isNaN(a)) {
                    handle_input_error();
                    return;
                } else
                {
                    input.errorOccured = false;
                }

                // Get the slider value
                var b = lut.getValue(slider.value);

                var op = getOperation();
                // Check for zero division
                if (op === "/" && b === 0) {
                        handle_slider_error();
                        return;
                } else {
                    slider.errorOccured = false;
                }
                
                // Get the result value
                let res = 0;
                switch(op) {
                    case "+":
                        res = a + b;
                        break;
                    case "-":
                        res = a - b;
                        break;
                    case "*":
                        res = a * b;
                        break;
                    case "/":
                        res = a / b;
                        break;
                    default:
                        break;
                }

                // Display the result value
                displayValue.text = res;
                displayValue.horizontalAlignment = Text.AlignHCenter;
            }
        }

        // Prvek pro zobrazení výsledné hodnoty
        Rectangle {
            height: 45;
            width: 400;
            border.color: "#bbb";
            border.width: 3;
            anchors.margins: 2
            color: "#777"
            
            Text {
                id:  displayValue;
                anchors.fill: parent;
                anchors.centerIn: parent
                height: 35;
                font.pointSize: 22
                color: "#0066FF"
            }
        }
    }

    // Vytvoření objektu LUT, který je definován v C++
    // K danému se přistupuje pomocí jeho id
    LUT {
        id: lut
    }

}

