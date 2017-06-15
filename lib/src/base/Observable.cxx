//                                               -*- C++ -*-
/**
 *  @brief Class defining object observable by the interface
 *
 *  Copyright 2015-2017 EDF-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "otgui/Observable.hxx"

#include <algorithm>

using namespace OT;

namespace OTGUI {

/* Default constructor */
Observable::Observable()
  : observers_()
  , blockedObserverType_("")
{
}


/* Copy constructor */
Observable::Observable(const Observable & other)
  : observers_()
  , blockedObserverType_("")
{
}


void Observable::addObserver(Observer * observer)
{
  observers_.push_back(observer);
}


void Observable::removeObserver(Observer * observer)
{
  observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
}


void Observable::notify(const String & message)
{
  // do not use for (std::vector<Observer*>::iterator it = observers_.begin(); it != observers_.end(); ++it)
  // avoid problem if an observer is added in the list observers_ in the update function
  const int nbObservers = observers_.size();
  for (int i=0; i<nbObservers; ++i)
  {
    if (observers_[i]->getType() != blockedObserverType_)
      observers_[i]->update(this, message);
  }
}


void Observable::notifyAndRemove(const String & message, const String & type)
{
  for (std::vector<Observer*>::iterator it = observers_.begin(); it != observers_.end(); ++it)
  {
    if ((*it)->getType() == type)
    {
      (*it)->update(this, message);
      observers_.erase(it);
      return;
    }
  }
}


void Observable::blockNotification(const String & blockedObserverType)
{
  blockedObserverType_ = blockedObserverType;
}


std::vector< Observer* > Observable::getObservers() const
{
  return observers_;
}


Observer* Observable::getObserver(const String & type)
{
  for (std::vector<Observer*>::iterator it = observers_.begin(); it != observers_.end(); ++it)
  {
    if ((*it)->getType() == type)
      return (*it);
  }
  return 0;
}
}