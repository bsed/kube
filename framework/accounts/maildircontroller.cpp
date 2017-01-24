/*
 *  Copyright (C) 2017 Michael Bohlender, <michael.bohlender@kdemail.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "maildircontroller.h"

#include <sink/store.h>

using namespace Sink;
using namespace Sink::ApplicationDomain;

MaildirController::MaildirController() : Kube::Controller(),
    action_create{new Kube::ControllerAction{this, &MaildirController::create}},
    action_modify{new Kube::ControllerAction{this, &MaildirController::modify}},
    action_remove{new Kube::ControllerAction{this, &MaildirController::remove}}
{

}

void MaildirController::create() {
    auto account = ApplicationDomainType::createEntity<SinkAccount>();
    account.setProperty("type", "maildir");
    account.setProperty("name", getName());
    //account.setProperty("icon", getIcon());
    Store::create(account).exec().waitForFinished();

    auto resource =  ApplicationDomainType::createEntity<SinkResource>();
    resource.setResourceType("sink.maildir");
    resource.setAccount(account);
    resource.setProperty("path", getPath().path());

    Store::create(resource).exec().waitForFinished();
}

void MaildirController::modify() {
    SinkResource resource(m_resourceId);
    resource.setProperty("path", getPath().path());
    Store::modify(resource).exec();
}

void MaildirController::remove() {
    SinkAccount account(m_accountId);
    Store::remove(account).exec();

    clear();
}

void MaildirController::load(const QByteArray &id) {

    m_accountId = id;
    clear();

    Store::fetchOne<SinkAccount>(Query().filter(m_accountId))
    .then([this](const SinkAccount &account) {
        setIcon(account.getIcon());
        setName(account.getName());
    }).exec();

    Store::fetchOne<SinkResource>(Query().filter<SinkResource::Account>(m_accountId).containsFilter<SinkResource::Capabilities>(ResourceCapabilities::Mail::storage))
    .then([this](const SinkResource &resource) {
        m_resourceId = resource.identifier();
        setPath(resource.getProperty("path").toString());
    }).exec();
}