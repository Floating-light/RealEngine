#pragma once 
#include "InputTypes.h"
#include "GenericPlatform/GenericApplication.h"
/**
 * Base class for all mouse and keyevents.
 */
struct RInputEvent
{
public:

	/**
	 * UStruct Constructor.  Not meant for normal usage.
	 */
	RInputEvent()
		: ModifierKeys(RModifierKeysState())
		, bIsRepeat(false)
	{ }

	/**
	 * Constructor.  Events are immutable once constructed.
	 *
	 * @param  InModifierKeys  Modifier key state for this event
	 * @param  bInIsRepeat  True if this key is an auto-repeated keystroke
	 */
	RInputEvent(const RModifierKeysState& InModifierKeys, bool bInIsRepeat)
		: ModifierKeys(InModifierKeys)
		, bIsRepeat(bInIsRepeat)
	{ }

	/**
	 * Virtual destructor.
	 */
	virtual ~RInputEvent( ) { }

public:

	/**
	 * Returns whether or not this character is an auto-repeated keystroke
	 *
	 * @return  True if this character is a repeat
	 */
	bool IsRepeat() const
	{
		return bIsRepeat;
	}

	/**
	 * Returns true if either shift key was down when this event occurred
	 *
	 * @return  True if shift is pressed
	 */
	bool IsShiftDown() const
	{
		return ModifierKeys.IsShiftDown();
	}

	/**
	 * Returns true if left shift key was down when this event occurred
	 */
	bool IsLeftShiftDown() const
	{
		return ModifierKeys.IsLeftShiftDown();
	}

	/**
	 * Returns true if right shift key was down when this event occurred
	 */
	bool IsRightShiftDown() const
	{
		return ModifierKeys.IsRightShiftDown();
	}

	/**
	 * Returns true if either control key was down when this event occurred
	 */
	bool IsControlDown() const
	{
		return ModifierKeys.IsControlDown();
	}

	/**
	 * Returns true if left control key was down when this event occurred
	 */
	bool IsLeftControlDown() const
	{
		return ModifierKeys.IsLeftControlDown();
	}

	/**
	 * Returns true if right control key was down when this event occurred
	 */
	bool IsRightControlDown() const
	{
		return ModifierKeys.IsRightControlDown();
	}

	/**
	 * Returns true if either alt key was down when this event occurred
	 */
	bool IsAltDown() const
	{
		return ModifierKeys.IsAltDown();
	}

	/**
	 * Returns true if left alt key was down when this event occurred
	 */
	bool IsLeftAltDown() const
	{
		return ModifierKeys.IsLeftAltDown();
	}

	/**
	 * Returns true if right alt key was down when this event occurred
	 */
	bool IsRightAltDown() const
	{
		return ModifierKeys.IsRightAltDown();
	}

	/**
	 * Returns true if either command key was down when this event occurred
	 */
	bool IsCommandDown() const
	{
		return ModifierKeys.IsCommandDown();
	}

	/**
	 * Returns true if left command key was down when this event occurred
	 */
	bool IsLeftCommandDown() const
	{
		return ModifierKeys.IsLeftCommandDown();
	}

	/**
	 * Returns true if right command key was down when this event occurred
	 */
	bool IsRightCommandDown() const
	{
		return ModifierKeys.IsRightCommandDown();
	}

	/**
	 * Returns true if caps lock was on when this event occurred
	 */
	bool AreCapsLocked() const
	{
		return ModifierKeys.AreCapsLocked();
	}

	/**
	 * Returns the complete set of modifier keys
	 */
	const RModifierKeysState& GetModifierKeys() const
	{
		return ModifierKeys;
	}

	/**
	* Returns the index of the user that generated this event.
	*/
	// uint32 GetUserIndex() const
	// {
	// 	return UserIndex;
	// }

	// const FWidgetPath* GetEventPath() const
	// {
	// 	return EventPath;
	// }

	// SLATECORE_API virtual FText ToText() const;
	
	/** Is this event a pointer event (touch or cursor). */
	virtual bool IsPointerEvent() const
	{
		return false;
	}

	/** Is this event a key event. */
	virtual bool IsKeyEvent() const
	{
		return false;
	}

protected:

	// State of modifier keys when this event happened.
	RModifierKeysState ModifierKeys;

	// True if this key was auto-repeated.
	bool bIsRepeat;

	// The index of the user that caused the event.
	// uint32 UserIndex;

	// Events are sent along paths. See (GetEventPath).
	// const FWidgetPath* EventPath;
};

struct RKeyEvent : public RInputEvent
{
public:
	/**
	 * UStruct Constructor.  Not meant for normal usage.
	 */
	RKeyEvent()
		: RInputEvent(RModifierKeysState(),false)
		, Key()
		, CharacterCode(0)
		, KeyCode(0)
	{
	}

	/**
	 * Constructor.  Events are immutable once constructed.
	 *
	 * @param  InKeyName  Character name
	 * @param  InModifierKeys  Modifier key state for this event
	 * @param  bInIsRepeat  True if this key is an auto-repeated keystroke
	 */
	RKeyEvent(	const RKey InKey,
				const RModifierKeysState& InModifierKeys, 
				const bool bInIsRepeat,
				const uint32_t InCharacterCode,
				const uint32_t InKeyCode
	)
		: RInputEvent(InModifierKeys, bInIsRepeat)
		, Key(InKey)
		, CharacterCode(InCharacterCode)
		, KeyCode(InKeyCode)
	{ }

	/**
	 * Returns the name of the key for this event
	 *
	 * @return  Key name
	 */
	RKey GetKey() const
	{
		return Key;
	}

	/**
	 * Returns the character code for this event.
	 *
	 * @return  Character code or 0 if this event was not a character key press
	 */
	uint32_t GetCharacter() const
	{
		return CharacterCode;
	}

	/**
	 * Returns the key code received from hardware before any conversion/mapping.
	 *
	 * @return  Key code received from hardware
	 */
	uint32_t GetKeyCode() const
	{
		return KeyCode;
	}

	virtual bool IsKeyEvent() const override
	{
		return true;
	}

private:
	// Name of the key that was pressed.
	RKey Key;

	// The character code of the key that was pressed.  Only applicable to typed character keys, 0 otherwise.
	uint32_t CharacterCode;

	// Original key code received from hardware before any conversion/mapping
	uint32_t KeyCode;
};

struct RPointerEvent : public RInputEvent
{
public:
    RPointerEvent(const Vector2D& InScreenSpacePosition, const Vector2D& InLastScreenSpacePosition, RKey InKey)
        : ScreenSpacePosition(InScreenSpacePosition)
        , LastScreenSpacePosition(InLastScreenSpacePosition)
        , Key(InKey)
    {

    }
	virtual bool IsPointerEvent() const override
	{
		return true;
	}
private:
    Vector2D ScreenSpacePosition;
    Vector2D LastScreenSpacePosition;
    RKey Key;
};
