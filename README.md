# Ready Player Me UE4 Third Person Character Example

![img-readyplayerme-character](https://user-images.githubusercontent.com/7085672/163189054-ceeda1d9-1982-4b39-9517-1bd758c746b3.png)

This repository contains an example Unreal Engine 4 project that creates a character controller using a Ready Player Me avatar. This is to serve as a reference for anybody that wants to integrate our 3D avatars into their Unreal Engine project. 

# Map

The ThirdPersonExampleMap is loaded on startup by default and can be found in the projects `Content > ReadyPlayerMe > Maps` folder as below.

![img-maps](https://user-images.githubusercontent.com/7085672/163190404-9bd95c07-9cf1-4dee-b6c3-1a5829be5d2c.png)

There is no custom code or actors in this map however if you open up the World Settings you will notice we set a GameMode Override to use our TP_GameMode asset. This game mode is used to set the Default Pawn Class to be our RPM_ThirdPersonCharacter.

![img-game-mode](https://user-images.githubusercontent.com/7085672/163191426-5f871707-eeba-4318-918f-12648419ea79.png)

# Character Blueprint

You can find our character blueprint named `RPM_ThirdPersonCharacter` in the projects `Content > ReadyPlayerMe > Blueprints` folder. 

This character was created simply by duplicating the example Unreal Third Person Character blueprint (that uses mannequin skeleton and mesh), then we reassigned the skeletal mesh and the animation blueprint as well as added a ReadyPlayerMeActorComponent to handle the loading of our ReadyPlayerMe avatars at runtime. 

# Animations 

If you look in the folder `Content > ReadyPlayerMe > Animations` you will find all the animations, blendspaces and animation blueprints used in our example.

![img-animations](https://user-images.githubusercontent.com/7085672/163196240-fa3186db-6fca-4ca4-8639-d0c5754490d7.png)

# Avatar Loader UI Widget

The Avatar Loader UI Widget get be found in `Content > ReadyPlayerMe > Widgets`.

![img-ui-widget](https://user-images.githubusercontent.com/7085672/163198531-ea688571-c43c-4931-8254-3fc49734865e.png)

When you run the application and press Q the UI panel should display as below.

![img-avatar-loader-ui](https://user-images.githubusercontent.com/7085672/163198553-23941483-28b8-4450-b206-faee776931d3.png)

# How to retarget animations

While our avatars use a skeleton rig based on the Mixamo skeleton, it is possible to retarget animations using the Mannequin skeleton so that they work with our Ready Player Me Avatars. For this example we did exactly that, to retarget animations from the Mannequn skeleton we first had to setup the bonemapping. To do this you need to open up the `UE4_Mannequin_Skeleton` asset located at `Content > Mannequin > Character > Mesh`. 

From there you need to open up the **Retarget Manager** panel and under the Set up rig menu, set `Select Rig = Humanoid`. Then click the AutoMap button and it will assign all the bones to the bonemap. Save your changes and navigate back to the content browser to find the animation, or animation blueprint you want to retarget. If you retarget an **Animation Blueprint** it will retarget and create a copy of every animation and blendspaces that is used in the blueprint as well as create a new Animation Blueprint. 

![img-retargeting-panel](https://user-images.githubusercontent.com/7085672/163196328-170b7e9d-fc30-47c5-8487-0e4d7c3fbbf2.png)

In our case we will use the `ThirdPerson_AnimBP` located at `Content > Mannequin > Animations` as an example. Simply right click and select `Retarget Anim Blueprints > Duplicate Anim Blueprints and Retarget`. 

![img-retarget-menu](https://user-images.githubusercontent.com/7085672/163198242-a774dbdd-b41b-46f7-ab2c-3d1d0af09441.png)

This will bring up the Select Skeleton window, in here simply select the `RPM_Mixamo_Skeleton` asset and click retarget. 

![img-select-skeleton](https://user-images.githubusercontent.com/7085672/163198391-ef5989e7-cda0-4cc5-8281-a6a515581a15.png)

Once the process is complete you should end up with animations and/or animation blueprints that work with our Ready Player Me avatar.

## Dependencies
- Ready Player Me Unreal SDK 
- glTFRuntime (Third party plugin)

## Links
**Ready Player Me Unreal SDK**
- [Documentation](https://link-url-here.org)
- [Download](https://docs.readyplayer.me/ready-player-me/integration-guides/unreal-engine-4/unreal-plugin-download)
- [Support](mailto:support@wolf3d.io)

**glTFRuntime**
- [Documentation](https://github.com/rdeioris/glTFRuntime-docs/blob/master/README.md)



