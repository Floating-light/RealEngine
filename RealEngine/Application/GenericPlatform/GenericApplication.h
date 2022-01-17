#pragma once
#include <memory>
#include "GenericWindow.h"
#include "GenericApplicationMessageHandler.h"

/**
 * RModifierKeysState stores the pressed state of keys that are commonly used as modifiers
 */
class RModifierKeysState
{

public:

	/**
	 * Constructor.  Events are immutable once constructed.
	 *
	 * @param  bInIsLeftShiftDown  True if left shift is pressed
	 * @param  bInIsRightShiftDown  True if right shift is pressed
	 * @param  bInIsLeftControlDown  True if left control is pressed
	 * @param  bInIsRightControlDown  True if right control is pressed
	 * @param  bInIsLeftAltDown  True if left alt is pressed
	 * @param  bInIsRightAltDown  True if right alt is pressed
	 */
	RModifierKeysState( const bool bInIsLeftShiftDown,
						const bool bInIsRightShiftDown,
						const bool bInIsLeftControlDown,
						const bool bInIsRightControlDown,
						const bool bInIsLeftAltDown,
						const bool bInIsRightAltDown,
						const bool bInIsLeftCommandDown,
						const bool bInIsRightCommandDown,
						const bool bInAreCapsLocked)
		: bIsLeftShiftDown( bInIsLeftShiftDown ),
		  bIsRightShiftDown( bInIsRightShiftDown ),
		  bIsLeftControlDown( bInIsLeftControlDown ),
		  bIsRightControlDown( bInIsRightControlDown ),
		  bIsLeftAltDown( bInIsLeftAltDown ),
		  bIsRightAltDown( bInIsRightAltDown ),
		  bIsLeftCommandDown( bInIsLeftCommandDown ),
		  bIsRightCommandDown( bInIsRightCommandDown ),
		  bAreCapsLocked( bInAreCapsLocked )
	{
	}

	RModifierKeysState()
		: bIsLeftShiftDown(false),
		  bIsRightShiftDown(false),
		  bIsLeftControlDown(false),
		  bIsRightControlDown(false),
		  bIsLeftAltDown(false),
		  bIsRightAltDown(false),
		  bIsLeftCommandDown(false),
		  bIsRightCommandDown(false),
		  bAreCapsLocked(false)
	{
	}
	
	/**
	 * Returns true if either shift key was down when this event occurred
	 *
	 * @return  True if shift is pressed
	 */
	bool IsShiftDown() const
	{
		return bIsLeftShiftDown || bIsRightShiftDown;
	}

	/**
	 * Returns true if left shift key was down when this event occurred
	 *
	 * @return  True if left shift is pressed
	 */
	bool IsLeftShiftDown() const
	{
		return bIsLeftShiftDown;
	}

	/**
	 * Returns true if right shift key was down when this event occurred
	 *
	 * @return  True if right shift is pressed
	 */
	bool IsRightShiftDown() const
	{
		return bIsRightShiftDown;
	}

	/**
	 * Returns true if either control key was down when this event occurred
	 *
	 * @return  True if control is pressed
	 */
	bool IsControlDown() const
	{
		return bIsLeftControlDown || bIsRightControlDown;
	}

	/**
	 * Returns true if left control key was down when this event occurred
	 *
	 * @return  True if left control is pressed
	 */
	bool IsLeftControlDown() const
	{
		return bIsLeftControlDown;
	}

	/**
	 * Returns true if right control key was down when this event occurred
	 *
	 * @return  True if right control is pressed
	 */
	bool IsRightControlDown() const
	{
		return bIsRightControlDown;
	}

	/**
	 * Returns true if either alt key was down when this event occurred
	 *
	 * @return  True if alt is pressed
	 */
	bool IsAltDown() const
	{
		return bIsLeftAltDown || bIsRightAltDown;
	}

	/**
	 * Returns true if left alt key was down when this event occurred
	 *
	 * @return  True if left alt is pressed
	 */
	bool IsLeftAltDown() const
	{
		return bIsLeftAltDown;
	}

	/**
	 * Returns true if right alt key was down when this event occurred
	 *
	 * @return  True if right alt is pressed
	 */
	bool IsRightAltDown() const
	{
		return bIsRightAltDown;
	}
	
	/**
	 * Returns true if either command key was down when this event occurred
	 *
	 * @return  True if command is pressed
	 */
	bool IsCommandDown() const
	{
		return bIsLeftCommandDown || bIsRightCommandDown;
	}
	
	/**
	 * Returns true if left command key was down when this event occurred
	 *
	 * @return  True if left command is pressed
	 */
	bool IsLeftCommandDown() const
	{
		return bIsLeftCommandDown;
	}
	
	/**
	 * Returns true if right command key was down when this event occurred
	 *
	 * @return  True if right command is pressed
	 */
	bool IsRightCommandDown() const
	{
		return bIsRightCommandDown;
	}


	/**
	 * @return  true if the Caps Lock key has been toggled to the enabled state.
	 */
	bool AreCapsLocked() const
	{
		return bAreCapsLocked;
	}

	/**
	 * @param ModifierKeys the modifier keys to test to see if they are pressed.  Returns true if no modifiers are specified.
	 * @return true if all modifier keys are pressed specified in the modifier keys.
	 */
	// bool AreModifersDown(EModifierKey::Type ModiferKeys) const
	// {
	// 	bool AllModifersDown = true;

	// 	if ( (ModiferKeys & EModifierKey::Shift) == EModifierKey::Shift )
	// 	{
	// 		AllModifersDown &= IsShiftDown();
	// 	}
	// 	if ( (ModiferKeys & EModifierKey::Command) == EModifierKey::Command )
	// 	{
	// 		AllModifersDown &= IsCommandDown();
	// 	}
	// 	if ( (ModiferKeys & EModifierKey::Control) == EModifierKey::Control )
	// 	{
	// 		AllModifersDown &= IsControlDown();
	// 	}
	// 	if ( (ModiferKeys & EModifierKey::Alt) == EModifierKey::Alt )
	// 	{
	// 		AllModifersDown &= IsAltDown();
	// 	}

	// 	return AllModifersDown;
	// }

private:

	/** True if the left shift key was down when this event occurred. */
	uint16_t bIsLeftShiftDown:1;

	/** True if the right shift key was down when this event occurred. */
	uint16_t bIsRightShiftDown:1;

	/** True if the left control key was down when this event occurred. */
	uint16_t bIsLeftControlDown:1;

	/** True if the right control key was down when this event occurred. */
	uint16_t bIsRightControlDown:1;

	/** True if the left alt key was down when this event occurred. */
	uint16_t bIsLeftAltDown:1;

	/** True if the right alt key was down when this event occurred. */
	uint16_t bIsRightAltDown:1;
	
	/** True if the left command key was down when this event occurred. */
	uint16_t bIsLeftCommandDown:1;
	
	/** True if the right command key was down when this event occurred. */
	uint16_t bIsRightCommandDown:1;

	/** True if the Caps Lock key has been toggled to the enabled state. */
	uint16_t bAreCapsLocked:1;
};


class RGenericApplication
{
public:
    virtual std::shared_ptr<RGenericWindow> MakeWindow() {return nullptr;};    
    virtual void InitlializeWindow(std::shared_ptr<RGenericWindow> InWindow,const GenericWindowDesc& desc) {} ;
    void SetMessageHandler(const std::shared_ptr<GenericApplicationMessageHandler>& InHandler);
    virtual RModifierKeysState GetModifierKeys() const { return RModifierKeysState();}
protected:
    std::shared_ptr<GenericApplicationMessageHandler> MessageHandler;
};

inline void RGenericApplication::SetMessageHandler(const std::shared_ptr<GenericApplicationMessageHandler>& InHandler)
{
    MessageHandler = InHandler;
}
