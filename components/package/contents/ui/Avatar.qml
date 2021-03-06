/*
  Copyright (C) 2016 Michael Bohlender, <michael.bohlender@kdemail.net>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

import QtQuick 2.7
import QtQuick.Layouts 1.1


Rectangle {

    property string name;

    //colors taken from https://techbase.kde.org/Projects/Usability/HIG/Color
    function calcColor(x)
    {
        switch (x % 5) {
        case 0:
            return "#16a085"
        case 1:
            return "#27ae60"
        case 2:
            return "#2980b9"
        case 3:
            return "#8e44ad"
        case 4:
            return "#c0392b"
        }
    }

    radius: 100

    color: calcColor(name.length)

    Text {
        anchors.centerIn: parent

        text: name.charAt(0)

        color: "#ecf0f1"

        font.capitalization: Font.AllUppercase
    }
}
