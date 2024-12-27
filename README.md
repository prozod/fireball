<img alt="logofireball" src="/assets/fireball.png" height="100" width="auto">

> UNDER DEVELOPMENT

A minimalistic implementation of 2D game. Inspired by the classic 1979 Asteroids arcade game and various
platformers.

- C++20
- OpenGL 3.3
- GLAD (version 4.5)
- GLFW3.4 (compiled as a static library)
- GLM 1.0

---

### What is the game about?

#### Dodge the Inferno

Dive into a fiery frenzy! As a nimble and helpless hero, you'll dodge and weave through a barrage of flaming
projectiles.

Each successful evasion fuels your internal power, making the next encounter even more intense.
But beware, three fateful strikes, and your journey ends! Can you master the inferno and emerge victorious?

What's the highest score obtained? Can you get past 100 points?

<img src="/assets/game.png" alt="game picture" height="600" width="auto">

---

### Keyboard controls

- ESCAPE - close game
- W/A/S/D - movement (WA, SD etc. - for strafing)
- LEFT_SHIFT - slow walk
- SPACE_KEY - short speed boost

---

### To do

- [x] Rendering 2D shapes
- [x] Using keyboard events/animating shapes
- [x] Loading textures
- [x] Context window boundaries
- [x] Collision detection
- [x] Flash screen on projectile hit and/or death
- [ ] Glyph/text rasterization
- [ ] Batch rendering sprites (ball, character movement etc.)
- [ ] Clean/organize & abstract some of the code

### To add (ideas)

- [ ] Collecting tokens off the playground, which give bonuses (extra speed, shield for next projcetile hit etc.)
- [ ] Different levels [jump up different structures with a parallax background while having to aviod
  the projectiles *(death = loss, get to the top = next level)*]
- [ ] Improve character actions/abilities the further you go (able to get a shield and dodge projectiles - *this idea
  ties with the first one above*)
- [ ] Better, more seamless-looking textures
- [ ] Game menu
