#include <JuceHeader.h>

class TextComponent : public juce::Component
{
public:
    TextComponent(const juce::String& textToDisplay);
    void paint(juce::Graphics& g) override;
    void setText(const juce::String& newText);

private:
    juce::String text;
};