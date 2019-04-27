//
//  SigDisplayWindow.h
//  SimpleMapperSigGUI
//
//  Created by johnty on 2019-04-16.
//  based off JUCE demo with transparent window and bouncing balls
//
//

#ifndef SigDisplayWindow_h
#define SigDisplayWindow_h


class SigDisplayComponent : public Component,
public Timer
{
public:
    SigDisplayComponent(String name="sig_name") : sigName(name)
    {
        setInterceptsMouseClicks (false, false);
        
        Random random;
        
        auto size = 10.0f + random.nextInt (30);
        
        ballBounds.setBounds (random.nextFloat() * 100.0f,
                              random.nextFloat() * 100.0f,
                              size, size);
        
        direction.x = random.nextFloat() * 8.0f - 4.0f;
        direction.y = random.nextFloat() * 8.0f - 4.0f;
        
        colour = Colour ((juce::uint32) random.nextInt())
        .withAlpha (0.5f)
        .withBrightness (0.7f);
        
        startTimer (60);
    }
    
    void setSize(float w, float h)
    {
        ballBounds.setSize(50+2.5*w, 50+2.5*h);
    }
    
    void paint (Graphics& g) override
    {
        g.setColour (colour);
        g.fillEllipse (ballBounds - getPosition().toFloat());
        
        g.setFont (7.0f);
        g.setColour (Colours::white);
        g.drawText(sigName, 0, 0, 120, 5, Justification::left);
    }
    
    void timerCallback() override
    {
        ballBounds += direction;
        
        if (ballBounds.getX() < 0)                      direction.x =  std::abs (direction.x);
        if (ballBounds.getY() < 0)                      direction.y =  std::abs (direction.y);
        if (ballBounds.getRight()  > getParentWidth())  direction.x = -std::abs (direction.x);
        if (ballBounds.getBottom() > getParentHeight()) direction.y = -std::abs (direction.y);
        
        setBounds (ballBounds.getSmallestIntegerContainer());
    }
    
    String getName() {return sigName;}
    
private:
    Colour colour;
    Rectangle<float> ballBounds;
    Point<float> direction;
    String sigName;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SigDisplayComponent)
};

class SigDisplayWindow : public Component
{
public:
    SigDisplayWindow (int numBalls)
    {
        for (int i = 0; i < numBalls; ++i)
        {
            auto* newBall = new SigDisplayComponent();
            newBall->setSize(10,10);
            balls.add (newBall);
            addAndMakeVisible (newBall);
        }
    }
    
//    SigDisplayWindow(String name)
//    {
//        devName = name;
//    }
    
    void addBall(String name="signal_name")
    {
        auto* newBall = new SigDisplayComponent(name);
        newBall->setSize(25,25);
        balls.add(newBall);
        addAndMakeVisible(newBall);
        DBG("new ball name = "<<name);
    }
    
    void setBallSize(float size, int idx=0)
    {
        if (idx<balls.size()) {
            balls[idx]->setSize(size, size);
        }
    }
    
    void setBallSize(float size, String name) //not the most efficient way to do this
    {
        for (int i=0; i<balls.size(); i++)
        {
            if (balls[i]->getName().equalsIgnoreCase(name))
            {
                balls[i]->setSize(size,size);
            }
        }
    }
    
    void removeBall() {
        balls.removeLast();
    }
    
    void remBall(String name) {
        for (int i=0; i<balls.size(); i++) {
            if (balls[i]->getName() == name) {
                balls.remove(i);
            }
        }
    }
    
    void mouseDown (const MouseEvent& e) override
    {
        dragger.startDraggingComponent (this, e);
    }
    
    void mouseDrag (const MouseEvent& e) override
    {
        // as there's no titlebar we have to manage the dragging ourselves
        dragger.dragComponent (this, e, nullptr);
    }
    
    void paint (Graphics& g) override
    {
        if (isOpaque())
            g.fillAll (Colours::white);
            else
                g.fillAll (Colours::blue.withAlpha (0.2f));
                
                g.setFont (16.0f);
                g.setColour (Colours::black);
                g.drawFittedText ("Each ball = signal\n size = value",
                                  getLocalBounds().reduced (8, 0),
                                  Justification::centred, 5);
                
                g.drawRect (getLocalBounds());
                }
    
private:
    ComponentDragger dragger;
    OwnedArray<SigDisplayComponent> balls;
    String devName;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SigDisplayWindow)
};


#endif /* SigDisplayWindow_h */
