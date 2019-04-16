/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    
    
    myMapperInput = new MapperInputThread();
    myMapperInput->addChangeListener(this);
    myMapperInput->addActionListener(this);
    DBG("MAIN: start mapper thread...\n");
    myMapperInput->startThread();
    DBG("MAIN: thread started!\n");
    
    addAndMakeVisible(sigSlider);
    sigSlider.setRange(0, 10.0);
    addAndMakeVisible(sigSlider1);
    sigSlider1.setRange(0, 10.0);
    addAndMakeVisible(sigSlider2);
    sigSlider2.setRange(0, 10.0);
    addAndMakeVisible(sigSlider3);
    sigSlider3.setRange(0, 10.0);
    //sigSlider.setTextValueSuffix("(units)");
    addAndMakeVisible(sigLabel);
    sigLabel.setText("inputvis", dontSendNotification);
    sigLabel.attachToComponent(&sigSlider, true);
    
    //sig display window
    mySigDisplay = new SigDisplayWindow(0);
    mySigDisplay->addToDesktop (ComponentPeer::windowIsTemporary);
    //windows.add (mySigDisplay);
    
    setSize (600, 400);
    
    Rectangle<int> area (0, 0, 400, 400);
    RectanglePlacement placement (RectanglePlacement::xLeft
                                  | RectanglePlacement::yBottom
                                  | RectanglePlacement::doNotResize);
    
    Rectangle<int> result(this->getRight(), this->getY(), area.getWidth(), area.getHeight());
    //auto result = placement.appliedTo (area, Desktop::getInstance().getDisplays()
    //                                   .getMainDisplay().userArea.reduced (20));
    
    mySigDisplay->setBounds (result);
    mySigDisplay->setVisible (true);
    
    
}

MainComponent::~MainComponent()
{
    DBG("Main destructor\n");
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //g.setFont (Font (16.0f));
    //g.setColour (Colours::white);
    //g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    auto sliderLeft = 50;
    sigSlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10 , 25);
    sigSlider1.setBounds(sliderLeft, 40, getWidth() - sliderLeft - 10 , 25);
    sigSlider2.setBounds(sliderLeft, 60, getWidth() - sliderLeft - 10 , 25);
    sigSlider3.setBounds(sliderLeft, 80, getWidth() - sliderLeft - 10 , 25);
}

void MainComponent::changeListenerCallback(ChangeBroadcaster *source)
{
    double val = myMapperInput->getLastVal();
    //DBG("main component update! val="<<val);
    //sigSlider.setValue(val);
    const double* vals = myMapperInput->getLastVals();
    sigSlider.setValue(vals[0]);
    sigSlider1.setValue(vals[1]);
    sigSlider2.setValue(vals[2]);
    sigSlider3.setValue(vals[3]);
    
}

void MainComponent::actionListenerCallback(const String &message)
{
    bool is_dev = false, is_map = false, is_sig = false, is_add = false, is_mod = false, is_rem = false;
    DBG(message);
    if (message.contains("dev")) is_dev = true;
    if (message.contains("sig")) is_sig = true;
    if (message.contains("map")) is_map = true;
    
    //there must be a more elegant way to do this...
    mapper_record_event type = (mapper_record_event)std::strtod(message.substring(4).toUTF8(), nullptr);
    //DBG("type:"<<type);
    switch (type) {
        case MAPPER_ADDED:
            is_add = true;
            
            break;
        case MAPPER_REMOVED:
            is_rem = true;
            break;
        case MAPPER_MODIFIED:
            is_mod = true;
            break;
        default:
            break;
    }
    
    if (is_map && is_add) {
        mySigDisplay->addBall();
    }
    if (is_map && is_rem) {
        mySigDisplay->removeBall();
    }
    
    
}
