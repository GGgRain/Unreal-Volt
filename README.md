<div align="center">
  <image src="https://github.com/user-attachments/assets/6e5eecb0-836b-4c47-a80f-970acdc6c828" width="300" height="300">
</div>


<div align="center">
  <h1>Volt</h1>
  <h3>The Slate Architecture Animation System for Unreal Engine</h3>
</div>

<p align="center">
  <a href="https://github.com/GGgRain/Unreal-Volt/fork" target="_blank">
    <img src="https://img.shields.io/github/forks/GGgRain/Unreal-Volt?" alt="Badge showing the total of project forks"/>
  </a>

  <a href="https://github.com/GGgRain/Unreal-Volt/stargazers" target="_blank">
    <img src="https://img.shields.io/github/stars/GGgRain/Unreal-Volt?" alt="Badge showing the total of project stars"/>
  </a>

  <a href="https://github.com/GGgRain/Unreal-Volt/blob/master/LICENSE.md" target="_blank">
    <img alt="Badge showing project license type" src="https://img.shields.io/github/license/GGgRain/Unreal-Volt?color=f85149">
  </a>

  <a href="https://discord.gg/DzNFax2aBS">
    <img src="https://img.shields.io/discord/977755047557496882?logo=discord&logoColor=white" alt="Chat on Discord">
  </a>

  <a href="https://gggrain.github.io/Unreal-Joint-Documentation/docs/category/volt">
    <img src="https://img.shields.io/badge/release%20notes-00B2EE.svg" alt="Release Notes">
  </a>

</p>


<p align="center">
  <a href="#mag-about">About</a> &#xa0; | &#xa0;
  <a href="#clipboard-main-features">Main Features</a> &#xa0; | &#xa0;
  <a href="#inbox_tray-installation">Installation</a> &#xa0; | &#xa0;
  <a href="#checkered_flag-quickstart">Quickstart</a> &#xa0; | &#xa0;
  <a href="#camera-usecases">Usecases</a> &#xa0; | &#xa0;
  <a href="#loudspeaker-supports">Supports</a>
</p>


## :mag: About ##

https://github.com/user-attachments/assets/035eedc0-65da-45d3-8e9c-62b19637d97b

https://github.com/user-attachments/assets/b06b12a8-6507-469d-a76c-543feb72c3fd

Volt is a modular animation system for Unreal Slate Architecture powered by various UObject-based modules that can manipulate slate properties as you want in both runtime and editor, allowing you to declare animations very quickly, intuitively and easily with unique declarative animation syntax.

Volt lets you make detailed slate animations that are well-structured and lightweight in performance - while keeping development(animating) very intuitive, easy, and fast.

This project was part of [Joint Project](https://www.unrealengine.com/marketplace/ko/product/ec432b9261c94b70a4068507d42a4f5a), a powerful full-modular conversation scripting framework, especially Volt took place in the editor module to animate the slates on the editor without limitation on the production and performances.

The project's distribution intends to enable you to use it on your editor for the visual part. **Let's make Unreal's editor beautiful!**

<p align="center">
<i>Please consider leaving a star if you loved this project! </i>⭐
</p>


## :clipboard: Main Features ##

### 1. Intuitive Declarative Animation

Volt allows users to animate slates with unique and intuitive animation declaration syntax. This helps you to quickly test out and bulid any animation production with slates.

https://github.com/user-attachments/assets/3f7a7471-34ed-45c5-913b-9854ed04af41

### 2. Simple, Thus Powerful Usages

Animating slates with Volt is **remarkably** easy - You might think it's illegal to do this easily!

You don't need to do any modification to the existing slate types to make it work with Volt - It will work with any slate types as they are.

If you want to animate SBorder instance? Just put that in ```VOLT_PLAY_ANIMATION()``` then it will magically create all the necessary objects internally - You don't need to care about them. **All you have to know is how to declare animations and use each modules to make animations as you want.**

```cpp
// Make SBorder slate here.
ChildSlot[
	SAssignNew(Border, SBorder)
	.BorderBackgroundColor(OutlineNormalColor)
	[
		...
	]
];

// Declare a volt animation
UVoltAnimation* Anim = VOLT_MAKE_ANIMATION(UVoltAnimation)
(
	VOLT_MAKE_MODULE(UVolt_ASM_InterpBackgroundColor)
	.TargetColor(OutlinePressColor)
);

// Play the animation with that SBorder slate. It's just like this.
VOLT_PLAY_ANIM(AnimationManager, Border, BorderColorAnim);
```

Considering the main reason why there are not so many plugins or system with animated UI are mostly come from the complexity of the vanila Unreal Engine's Slate animation development process (It's horribly slow and difficult to achieve), The level of simplexity of the usage of Volt will let you make much more detailed and creative visual productions.


### 3. Light-Weighted Performance & Multithreading Support

Volt has been designed to be fast and lighted-weighted on the performance and memory aspect

It tries to reduce the amount of memory usage, total object counts and iteration counts internally with a ton of performance related optimization logic - including unique garbage collecting logic that works with ownership system, selective variables implementation...

Plus, Volt never makes bottleneck on the game thread, because it works with multithreading - many of the intensive actions are all done in a independent thread. This lets users to focus solely on the animation development while not caring too much about the performance issue.

Also, Many features including querying slate instances or animation tracks are designed to be fast enough ( O(1) ) to supports maximum performance - thus Volt could be practical enough for every project.

## :inbox_tray: Installation ##

We assume that you are going to use Volt on your plugin.

1. Download and unzip the plugin.
2. Copy-paste the contents in the Source folder to your plugin.
3. Go to your plugin's .uplugin file and add the modules there. (Volt, VoltCore, VoltEditor). Please check out the Volt.uplugin to see the module type and loading phase for each module.
4. And **MOST IMPORTANTLY**, **rename all the classes and module names to something unique, to prevent compatibility issues with other programs that use Volt too. Unfortunately, Epic Games forbid us to make the plugin dependent on the plugin on the marketplace so we can not share libraries between multiple projects unless it becomes a part of the engine itself.**

### Modules

* Volt - The module that contains some basic ready-to-use modules and variables for the plugin.
* VoltCore - The module that contains the fundamental classes for the framework.
* VoltEditor - The module that contains editor only classes, including **VoltStarship (Showcase + Boilerplate)**

> [!NOTE]
> If you are working on the actual project, we recommend to exclude VoltEditor module in the final bulid.


## :checkered_flag: Quickstart ##

### Making Animation Manager

To animate slates in Volt, you have to make a ```UVoltAnimationManager``` instance first that will actually manage all the animation tracks. 

> [!NOTE]
> One ```UVoltAnimationManager``` can animate multiple slates as you want, So it's up to you whether to make ```UVoltAnimationManager``` per each slate or reuse any external, already existing instance to save memory usage.

You can make a ```UVoltAnimationManager``` instance with macro: ```VOLT_IMPLEMENT_MANAGER( AnimationManagerPropertyAddress, OwnerSlateSharedPtr )``` or ```VOLT_IMPLEMENT_MANAGER( AnimationManagerPropertyAddress, OwnerUObject )```

```cpp
// In SVoltSampleStarshipTab.h...
UVoltAnimationManager* AnimationManager = nullptr;

// In SVoltSampleStarshipTab.cpp...
VOLT_IMPLEMENT_MANAGER(&AnimationManager, SharedThis(this));
```
### Declaring Animation & Modules

Now its time to make an actual animation instance. You can use ```VOLT_MAKE_ANIMATION( AnimationClassName )``` macro to declare a new animation with the provided type animation (typically ```UVoltAnimation```) and ```VOLT_MAKE_MODULE( ModuleClassName )``` macro to create a module for the provided module class.

And you can wrap modules with ```( )``` to feed the modules to the animation, then the animation will get those modules and use them to animate the slate.

> [!TIP]
> ```() operator``` (wraping modules with ```( )```) also works for the modules that can have submodules on providing submodules for them.  
> In Volt 1.1, we have 3 object types that take submodules, ```UVoltAnimation```, ```UVolt_ASM_Sequence```, ```UVolt_ASM_Simultaneous```.

In this example, You can see that ```UVoltAnimation``` object will be created with ```UVolt_ASM_Sequence``` inside, that has 2 submodules (```UVolt_ASM_InterpWidgetTransform```) that will animate the slate's render transform as provided.

You can also see that this animation will works like: changing a slate's render transform to make it move and forth (```UVolt_ASM_InterpWidgetTransform```, first one will animate the slate's X location like -50 -> 50, second one will animate it 50 -> -50 )  continuously. (```UVolt_ASM_Sequence```, loop has been enabled by 	.bShouldLoop(true)
)

```cpp
UVoltAnimation* Animation = VOLT_MAKE_ANIMATION(UVoltAnimation) (
	VOLT_MAKE_MODULE(UVolt_ASM_Sequence)
	.bShouldLoop(true)
	(
		VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
		.bUseStartWidgetTransform(true)
		.StartWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
		.TargetWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
		.RateBasedInterpSpeed(3),
		VOLT_MAKE_MODULE(UVolt_ASM_InterpWidgetTransform)
		.bUseStartWidgetTransform(true)
		.StartWidgetTransform(FWidgetTransform(FVector2D(50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
		.TargetWidgetTransform(FWidgetTransform(FVector2D(-50, 0), FVector2D(1, 1), FVector2D(0, 0), 0))
		.RateBasedInterpSpeed(3),
	)
)
```

### Playing Animation

Only the final process is left! Let's play the animation we made. Let's use ```VOLT_PLAY_ANIM( AnimationManagerPtr, SlateToAnimate, Animation )``` macro to animate the slate.

```cpp
VOLT_PLAY_ANIM(AnimationManager, SharedThis(this), Animation);
```

Then you will see something like this.
<div align="center">
    <image src="https://github.com/user-attachments/assets/ba7eeb57-b16a-4ab1-967e-0c7b522310f4" width="500">
</div>

### Learning Further...

Volt provides all the other features making & managing slates much organized manner. 

> [!IMPORTANT]
> In this document, we will cover how to use specific macros that are most useful in general usages only. If you need any further help, Please join the official discord!  
> Or you can find out some release notes to learn about the features - whenever we have anything to explain about them, we leave very detailed explanations for the changes and features.



#### Storing Animation Track

You can store ```FVoltAnimationTrack``` from ```VOLT_PLAY_ANIM( ... )``` macro that indicate the track of the animation playing in the animation manager. You can use this track to manage (query, check playback state, stop) the playing animation.

```cpp
FVoltAnimationTrack& Track = VOLT_PLAY_ANIM(AnimationManager, SharedThis(this), Animation);
```


#### Stopping Animation

You can use ```VOLT_STOP_ANIM( AnimationManagerPtr, AnimationTrack )``` to stop specific animation from specific animation manager with its track.

```cpp
VOLT_STOP_ANIM(AnimationManager, Track );
```


#### Checking Animation State

You can use ```VOLT_CHECK_PLAYING_ANIM( AnimationManagerPtr, AnimationTrack )``` to check whether specific animation is still being played or finished.

```cpp
if (VOLT_STOP_ANIM(AnimationManager, Track )) {
    // logic when animation is still being played...
}
```

#### Finding Volt Interface For Slate

You can find corresponding ```IVoltInterface``` for specific slate with ```VOLT_FIND_OR_ASSIGN_INTERFACE_FOR( SlateWeakPtr )``` macro.

Querying uses hash table internally - guaranteed to provide O(1) time efficiency (at good condition) - you can spam it as you want. Don't be shy!

For native slate types, It will return ```UVoltProxy``` object that wraps the provided slate as a return.

> [!TIP]
> Volt internally use ```IVoltInterface``` as the unit of the object that can be animated with Volt system, that must provide ```UVoltVariableCollection``` instance that contains all the animated variables that are used to animate the slate, and also the TargetSlate that the interface will animate.  
> But for most cases, Slates aren't directly inherited from ```IVoltInterface```, so we wrap the slate with a proxy object called ```UVoltProxy``` that inherits ```IVoltInterface``` instead of that slate.  
> 
> This is why it returns ```UVoltProxy``` instead, and this is why the system can take any type of slates with no difficulty. It's not a black magic!
>




#### Releasing Animation Manager Explicitly

You can use ```VOLT_RELEASE_MANAGER( AnimationManagerPtrAddress )``` to explicitly release the target ```UVoltAnimationManager``` instance.

> [!NOTE]
> ```UVoltAnimationManager``` instances will be automatically GC'd by the system after some times even if you don't explicitly release it. You can check out UVoltSubsystem.cpp to learn more about clean up code if you want.  
> We also wrote clear instruction related to the GC logic in the UVoltSubsystem.h. You can check them out as well.

```cpp
VOLT_RELEASE_MANAGER(&AnimationManager);
```

#### Making New Animation Modules

Sometimes, default provided modules aren't sufficient for you to create your desired production. But you can always make new modules for your need!

However, this topic will too long for this document. Please visit this [release note](https://gggrain.github.io/Unreal-Joint-Documentation/docs/release_note/Volt/1_1_0) for further details.


## :camera: Usecases
[Joint](https://www.unrealengine.com/marketplace/ko/product/ec432b9261c94b70a4068507d42a4f5a), and [Joint Native](https://github.com/GGgRain/Joint-Native) is using this plugin to animated their editor slates. Please let us know if you want to share your great project with Volt!

## :loudspeaker: Supports
If you have any questions related to Volt, please visit our [official discord](https://discord.gg/DzNFax2aBS)

## :memo: License ##
This project is licensed under the MIT License. For more details, please refer to the [LICENSE](LICENSE.md) file.

