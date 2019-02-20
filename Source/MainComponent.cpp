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
    DBG("MAIN: start mapper thread...\n");
    myMapperInput->startThread();
    DBG("MAIN: thread started!\n");
    
    addAndMakeVisible(sigSlider);
    sigSlider.setRange(0, 10.0);
    sigSlider.setTextValueSuffix("(units)");
    addAndMakeVisible(sigLabel);
    sigLabel.setText("label", dontSendNotification);
    sigLabel.attachToComponent(&sigSlider, true);
    
    setSize (600, 400);
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
    auto sliderLeft = 120;
    sigSlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10 , 25);
}

void MainComponent::changeListenerCallback(ChangeBroadcaster *source)
{
    double val = myMapperInput->getLastVal();
    DBG("main component update! val="<<val);
    sigSlider.setValue(val);
    
}
