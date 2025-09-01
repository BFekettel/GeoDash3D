# GeoDash 3D

A C++ 3D port of the popular game Geometry Dash.

---

## 📄 Document Purpose
This README documents the project as a whole — essentially a timeline and record of objectives, intentions, and how execution unfolded.

---

## 🎯 Project Goals
The goal for the project is to re-create the popular game **Geometry Dash** using the **OpenGL API** with **C++**, putting into practice the skills and knowledge gained during university.

This project is intended as a **portfolio piece**, not a finished commercial product. It’s a showcase of skills in **software** and **game development**.

---

## 🛠 Technical Goals
The technical aim is to use **advanced low-level programming techniques** to build a product that others can extend and modify.

**Other outcomes:**
- Deeper understanding and use of Graphics APIs.
- A technical showcase of low-level programming skills.
- Demonstration of Object-Oriented Programming paradigms, design patterns, and optimisations.
- Provide developers with the ability to clone and expand the project.

**Software Tools:**
- **C++ 20**
- **GLAD** / **GLFW** libraries
- **CLion** IDE
- **GitHub** for version control

**Inspiration:** Many of the system designs (components, events, objects, etc.) take inspiration from **Unreal Engine**.

---

## 🎮 Initial Game Outline
**GeoDash3D** takes heavy inspiration from *Geometry Dash*.  

**Core loop:**
- Guide the player’s character through platforms and obstacles to the level’s end.
- The character moves automatically — the player only controls jumps.
- The initial development goal is to replicate this loop with a cube character.

---

## 🧩 Initial Working Prototype Requirements

### Player (Cube Character)
- Jump on button input.
- Automatically move forward.
- Collision detection prevents falling through floors.
- Collision with objects:
  - If colliding front-on with most objects → reset level.
  - If colliding with spikes → instant reset.
- Winning condition when reaching the level’s end (display a win screen).

### Objects & Obstacles
- **Walls/Floors:** Player can stand on top but not pass through front-on.
- **Spikes:** Destroy the player on any collision.
- **Bounce Pads:** Launch the player higher than a normal jump.

### Level Editor
- In-game editor to create custom levels with built-in objects/obstacles.
- Save levels as `.G3D` binary files.
- Main menu allows loading levels via file explorer into the game’s directory.
- Option to edit loaded levels.

### Main Menu
- **Play** → opens level select menu.
- **Level Editor** → open editor mode.
- **Load Level** → load `.G3D` files.

### Level Select Menu
- Displays all loaded levels with preview thumbnails.

---

## 🖥 Technical Design
**UML Diagrams:** [View UML Design](https://drive.google.com/file/d/1Zo191hFVnJD_m2_beG95Pw8NEu7ZUg-m/view?usp=sharing)


## Learning Resources 
**OpenGL Shaders:** [Website](https://learnopengl.com/Getting-started/Shaders)

**OpenGL Renderer Concepts:** [Video](https://www.youtube.com/watch?app=desktop&v=jjaTTRFXRAk&t=3m11s)
This video discusses the OpenGL concept of a renderer. This was going to be an early issue I needed to solve before I got too deep, as I was working on the Static Shape Render I thought about the possible issue of sharing a camera system across all the instances of any Object that needed to be rendered in. The Cherno covers this concept in detail, giving me the idea of what I need to do before I go deeper in.
Changes from this:
-	Added Renderer Class to UML
-	Reworked code to use a global Renderer as a source for perspective rather than static “magic values”

**IMGUI:** [Repo](https://github.com/ocornut/imgui)
ImGUI will be a useful library to have, it provides a quick 2d element to the viewport, allowing me to display and edit values with ease. Initial use will be used for displaying performance metrics (fps and ms), but Ill like to play around with it more to see how it can help with debugging, such as toggles for show collision.

**OpenGL Anti-Aliasing** [Website](https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing)
This Website covers Anti Aliasing, not really needed but would be a nice addition to adopt early while developing the base rendering systems.


**

---

## 📌 Notes
- This document outlines a **prototype**; the final product will expand beyond this.
- More features and details will be added as development progresses.
