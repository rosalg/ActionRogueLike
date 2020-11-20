# ActionRogueLike

For my final project, I did as I said in my write up: players, when hitting the tab button, have the ability to look at a progression tree to level up skills that are dependent
on skills at lower points in the tree. You can use credits to purchase these skills, and on death, the purchased skills will persist.

One of the most significant challenges was figuring out how to exactly convey skills were unlocked in data. I initially started by just having a list of actions, but I quickly
realized that this was a clunky approach. I tried switching over to purely GameplayTags, but realized that gameplay tags alone didn't have the power to remember what action 
classes needed to be kept around after death. I finally settled on a map, mapping an unlocked gameplay tag (saying the player had already unlocked a given skill) to a Action 
subclass so that when a new pawn was spawned, it could just refer to this map.
