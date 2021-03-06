/*
    Copyright (c) 2016 Christian Mollekopf <mollekopf@kolabsys.com>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/
#pragma once

#include <QObject>
#include <QSharedPointer>

namespace Kube {

struct ActionResultData
{
    ActionResultData() : mError(0), mDone(false) {}
    int mError;
    bool mDone;
};

class ActionResult : public QObject
{
    Q_OBJECT
public:
    ActionResult() : QObject(), mData(new ActionResultData()) {}
    ActionResult(const ActionResult &rhs) : QObject(), mData(rhs.mData) {}
    ActionResult &operator=(const ActionResult &rhs)
    {
        mData = rhs.mData;
        return *this;
    }
    virtual ~ActionResult() {}

    ActionResult &operator+=(const ActionResult &rhs)
    {
        if (!error() && rhs.error()) {
            setError(rhs.error());
        }
        if (isDone() && rhs.isDone()) {
            mData->mDone = false;
        }
        mData = rhs.mData;
        return *this;
    }

    void setDone() {
        mData->mDone = true;
    }

    bool isDone() const {
        return mData->mDone;
    }

    void setError(int error) {
        mData->mError = error;
    }

    int error() const {
        return mData->mError;
    }

private:
    QSharedPointer<ActionResultData> mData;
};

}

Q_DECLARE_METATYPE(Kube::ActionResult);
