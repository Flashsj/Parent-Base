# Custom Base

This is a mod menu base made for Grand Theft Auto V and compatible with GTA Online aswell.

## Making a parent submenu
Parent submenus are the top submenus such as "Self" and "Vehicle".
You will have seen these in sub1to's projects such as 2Take1 and subVersion.

To add a new parent submenu, create a new function in MainScript.cpp with the name of your parent submenu and the word "Sub".
It should return nothing (void) and have no arguments.

The first line of your new function should be an integer variable assigned a call to add_parent_sub.
E.g. If your parent submenu was called "SelfSub", the first line in that function should be
```
int selfIndex = g_UIManagement->add_parent_sub("Self");
```

The second line of your new function should be an integer variable assigned a call to add_child_sub.
E.g.
```
int selfHomeIndex = g_UIManagement->m_ChildParentSubs[selfIndex].add_child_sub("Home");
```
This creates the "Home" sub menu inside the "Self" parent sub. If any parent sub has no child sub menus the game will crash.
m_ChildParentSubs is a vector of parent subs, and parent subs contain vectors of child subs which contain your options.

Now we are ready to add some options to our menu!

>>FOR THE REST OF THIS README, I EXPECT YOU ALSO MADE A VARIABLE TO STORE THE PARENT SUB CLASS OF THE CURRENT PARENT SUB. THIS CAN BE DONE BY
```
ParentSub* parentSub = &g_UIManagment->m_ChildParentSubs[selfIndex];
```

## Making a BooleanOption
Boolean options are options that can be turned on/off and manage a variable you assign them.

To make a boolean option, first create a boolean option somewhere e.g. ```int godModeBoolean = false;```

Then create your actual BoolOption:
```static BoolOption godModeBooleanOption{ "God mode", godModeBoolean };```
The first arg in the constructor is the display name of your option, and the second arg is the boolean variable you want to manage.
Finally, we need to add our boolean option to the submenu we want, in this case it is the "Home" submenu
```parentSub->m_ChildSubs[selfHomeIndex].add_option(godModeBoolean);```

## Making a RegularOption
Regular options are options where when clicked, they execute a single function as defined by yourself.

To make a regular option, first create a RegularOption variable
```static RegularOption suicideRegularOption{ "Easy way out", [] { ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0); } };```
Then just add it to the submenu we want like before, in this case it is the "Home" submenu
```parentSub->m_ChildSubs[selfHomeIndex].add_option(suicideRegularOption);```

## Making another child SubMenu and a SubOption
A sub option is an option which when clicked, they switch the current submenu to the desired one.

To make a sub option, first you must create a sub menu
```int selfWeaponsIndex = parentSub->add_child_sub("Weapons");```
We are going to make a submenu for weapon options.

Then you have to create your SubOption variable
```static SubOption selfWeaonsSubOption{ "Weapon options", selfIndex, selfWeaponsIndex };```
Here, the first arg is the display text for the option, the second arg is the parent submenu the sub is for, and the third arg is the child submenu you want to change to.

Finally, you need to add the sub option to the sub menu
```parentSub->m_ChildSubs[selfHomeIndex].add_option(selfWeaponsSubOption);```
