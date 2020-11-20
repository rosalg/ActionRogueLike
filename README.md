# ActionRogueLike

For my final project, I did as I said in my write up: players, when hitting the tab button, have the ability to look at a progression tree to level up skills that are dependent
on skills at lower points in the tree. You can use credits to purchase these skills, and on death, the purchased skills will persist.

One of the most significant challenges was figuring out how to exactly convey skills were unlocked in data. I initially started by just having a list of actions, but I quickly
realized that this was a clunky approach. I tried switching over to purely GameplayTags, but realized that gameplay tags alone didn't have the power to remember what action 
classes needed to be kept around after death. I finally settled on a map, mapping an unlocked gameplay tag (saying the player had already unlocked a given skill) to a Action 
subclass so that when a new pawn was spawned, it could just refer to this map.

Another challenge was implementing the UI itself. While formatting was relatively easy (after having worked in Unity's UI system and having nightmares to this day about it), 
managing events and the granularity with which buttons were programmed for functionality was a challenge. I initially thought about keeping every button separate, and having each 
button essentially implement it's own result, but quickly found this to be tedious and unscalable. I settled on just making everything an action, whether it was a one-time thing 
or a repeated thing, then adding any tags to remember the state.

I also made a grave mistake when it came to ensuring skills were remembered on death. Instead of implementing memory of unlocked skills in the PlayerState, I implemented it
directly in the ActionComponent which didn't work since the action component changed on death. To work around this, I create the aforementioned GameplayTag map then just reset
the actions whenever a new pawn was spawned.

If I was to extend this project, I would build in multiplayer support. Furthermore, I would try and outsource my code from blueprint to C++ to help optimization.
