/*
 * Copyright (C) 2016 Michael Bohlender <michael.bohlender@kdemail.net>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
//import org.kde.kube.mail 1.0 as Mail
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Item {
    id: root

    function send() {

    }

    function saveAsDraft() {

    }

    function clear() {

    }

    ColumnLayout {

        anchors.fill: parent

        GridLayout {

            columns: 2

            Label {
                text: "From"
            }

            ComboBox {
                model: ["Kuberich <kuberich@kolabnow.com>", "Uni <kuberich@university.edu>", "Spam <hello.spam@spam.to>"]

                Layout.fillWidth: true
            }

            Label {
                text: "To"
            }

            TextField {
                id: to

                Layout.fillWidth: true
            }

            Label {
                text: "Cc"
            }

            TextField {
                id: cc

                Layout.fillWidth: true
            }

            Label {
                text: "Bcc"
            }

            TextField {
                id: bcc

                Layout.fillWidth: true
            }
        }

        TextField {
            id: subject

            Layout.fillWidth: true

            placeholderText: "Subject"
        }

        TextArea {
            id: content

            Layout.fillWidth: true
            Layout.fillHeight: true

        }
    }
}