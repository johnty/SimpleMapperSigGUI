/*
  ==============================================================================

    MapperInputThread.cpp
    Created: 19 Feb 2019 10:36:05pm
    Author:  johnty

  ==============================================================================
*/

#include "MapperInputThread.h"

MapperInputThread::MapperInputThread(): Thread("MapperInputThread"),
                                        myMapperDev(new mapper::Device(DEVICE_NAME_STR)),
                                        myMapperDB(new mapper::Database(MAPPER_OBJ_ALL))
{
    //myMapperDev = new mapper::Device("inputvis");
    
    //mapper::Signal sig = myMapperDev->add_input_signal("in1", 1, 'f', 0, 0, 0, &MapperInputThread::sigUpdateHandler, (void*) this);
    //myInputSigs.push_back(&sig);
    
    
    myMapperDB->subscribe(MAPPER_OBJ_ALL);
    myMapperDB->add_device_callback(devActionHandler, this);
    myMapperDB->add_signal_callback(sigActionHandler, this);
    myMapperDB->add_map_callback(mapActionHandler, this);
    
}

MapperInputThread::~MapperInputThread()
{
    DBG("stopping mapper thread...\n");
    myMapperDB->unsubscribe();
    //delete myMapperDev;
    int res = stopThread(5000);
    DBG("thread stopped. res=" << res);
    
}

void MapperInputThread::run()
{
    DBG(" mapper thread begin\n");
    while (!threadShouldExit())
    {
        myMapperDev->poll(10);
        myMapperDB->poll(10);
    }
    DBG(" mapper thread end\n");
}


void MapperInputThread::devActionHandler(mapper_database db, mapper_device dev,
                            mapper_record_event action,
                            const void *user)
{
    //DBG("static devActionHandler");
    reinterpret_cast<const MapperInputThread*>(user)->devActionFn(dev, action);
}


void MapperInputThread::sigActionHandler(mapper_database db, mapper_signal sig,
                            mapper_record_event action,
                            const void *user)
{
    //DBG("static sigActionHandler");
    reinterpret_cast<const MapperInputThread*>(user)->sigActionFn(sig, action);
}

void MapperInputThread::mapActionHandler(mapper_database db,
                                      mapper_map map,
                                      mapper_record_event action,
                                      const void *user)
{
    //DBG("static mapActionHandler");
    reinterpret_cast<const MapperInputThread*>(user)->mapActionFn(map, action);
}


void MapperInputThread::devActionFn(mapper_device dev, mapper_record_event action) const
{
    //DBG("instance dev action!");
    String msg = "dev ";
    msg+= (int)action;
    sendActionMessage(msg);
    
    mapper::Device Dev(dev);
    String name = Dev.name();
    
//    if (!name.contains(DEVICE_NAME_STR))
//    {
//        DBG("creating new dev window " <<Dev.name());
//        auto* newDevWind = new SigDisplayWindow(Dev.name());
//        newDevWind->addToDesktop(ComponentPeer::windowIsTemporary);
//        //myDevWindows.add(newDevWind);
//    }
    
    
    
}

void MapperInputThread::sigActionFn(mapper_signal sig, mapper_record_event action) const
{
    //DBG("instance sig action!");
    //we use this to check for when an output signal is added
    // this will then trigger the creation of a listener signal
    
    mapper::Signal Sig(sig);
    bool isRem = false;
    if (action == MAPPER_REMOVED) isRem = true;
    
    DBG("sigAction - name: "<<Sig.name()<<" dir: "<<Sig.direction()<<" isRem: "<<(int)isRem);
    
    //
    
    if ((action == MAPPER_ADDED) && (Sig.direction() == MAPPER_DIR_OUTGOING)) {
        String name = Sig.device().name()+"/"+Sig.name();
        //name+= "_l";
        mapper::Signal newSig = myMapperDev->add_input_signal(name.toRawUTF8(), Sig.length(), 'f', 0, 0, 0, &MapperInputThread::sigUpdateHandler, (void*) this); //do we care about sig units, min/max etc?
        while (!myMapperDev->ready()) {
            myMapperDev->poll(50);
        }
        if ((mapper_signal)newSig != nullptr) {
            //DBG("trying to map "<<Sig.name()<<" to "<<newSig.name()<<"...");
            DBG("OUTPUT FOUND! adding new input sig: "<<newSig.name());
//            while (!myMapperDev->ready())
//                myMapperDev->poll(50);
             mapper::Map map(Sig, newSig);
             map.push(); //do we need to wait till ready?
             while (!map.ready()) {
                myMapperDev->poll(50);
             }
             DBG("\n mapped "<<Sig.name()<<"->"<<newSig.name());
        }
    }
    if ((action == MAPPER_REMOVED) && (Sig.direction() == MAPPER_DIR_OUTGOING)) {
        String name = Sig.device().name()+"/"+Sig.name();
        //String name = Sig.name();
        //name+= "_l";
        DBG("trying to remove "<<name);
        mapper::Signal mysig = myMapperDev->signal(name.toRawUTF8());
        if ((mapper_signal)mysig != nullptr) {
            myMapperDev->remove_signal(mysig);
            DBG(name<<"    removed!\n");
        }
        else {
            DBG(name<<"    not found!\n");
        }
    }
    
    //notify UI
    String msg = "sig ";
    msg+= (int)action;
    //sendActionMessage(msg);
}

void MapperInputThread::mapActionFn(mapper_map map, mapper_record_event action) const
{
    //DBG("instance map action!");
    //this is when a map is created
    //  we check here to see if it belongs to us, and if so, update the U
    //newmap.destination().signal().
    
    if (action== MAPPER_ADDED) {
        mapper::Map newmap = mapper::Map(map);
        mapper::Signal Sig = newmap.source().signal();
        String name = Sig.device().name()+"/"+Sig.name();
        String msg = "newmap "+ name;
        sendActionMessage(msg);
    }
    if (action==MAPPER_REMOVED) {
        mapper::Map newmap = mapper::Map(map);
        mapper::Signal Sig = newmap.source().signal();
        String name = Sig.device().name()+"/"+Sig.name();
        String msg = "delmap "+ name;
        sendActionMessage(msg);
    }
    
}

void MapperInputThread::sigUpdateHandler(mapper_signal sig, mapper_id instance, const void *value,
                       int count, mapper_timetag_t *timetag)
{
    //DBG("static sig update function");
    reinterpret_cast<MapperInputThread*>(mapper::Signal(sig).user_data())->sigUpdate(sig, instance, value, count, timetag);
}

void MapperInputThread::sigUpdate(mapper_signal sig, mapper_id instance, const void *value,
                       int count, mapper_timetag_t *timetag)
{
    //return;
    DBG("instance sig update function");
    
    mapper::Signal Sig(sig);
    String msg;
    
    if (value) {
        printf("--> destination got %s", mapper_signal_name(sig));
        int len = mapper_signal_length(sig);
        switch (mapper_signal_type(sig)) {
            case 'i': {
                int *v = (int*)value;
                for (int i = 0; i < len; i++) {
                    printf(" %d", v[i]);
                }
                lastVal = (double)v[0];
                DBG(" i; set to "<<lastVal);
                break;
            }
            case 'f': {
                float *v = (float*)value;
                for (int i = 0; i < len; i++) {
                    printf(" %f", v[i]);
                }
                lastVal = (double) v[0];
                //NOTE: the visualizer for now only uses the first value!!
                DBG(" f; set to "<<lastVal);
//                if (strcmp("in1",  mapper_signal_name(sig))==0)
//                    lastVals[0] = lastVal;
//                if (strcmp("in2",  mapper_signal_name(sig))==0)
//                    lastVals[1] = lastVal;
//                if (strcmp("in3",  mapper_signal_name(sig))==0)
//                    lastVals[2] = lastVal;
//                if (strcmp("in4",  mapper_signal_name(sig))==0)
//                    lastVals[3] = lastVal;
                break;
            }
            case 'd': {
                double *v = (double*)value;
                for (int i = 0; i < len; i++) {
                    printf(" %f", v[i]);
                }
                lastVal = v[0];
                DBG("set to "<<lastVal);
            }
        }
        printf("\n");

    }
    lastSigName = Sig.name();
    msg = "sig_changed "+lastSigName;
    sendActionMessage(msg);
    
    
    
    sendChangeMessage();
    
}
