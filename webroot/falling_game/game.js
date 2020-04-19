(function(w,d){

    var game = {}; // object for game stuff 

    // create an instance of the canvas and other useful objects
    const canvas = d.getElementById('canvas');
    const ctx = canvas.getContext('2d');
    const roomFloor =  Math.floor(0.7*w.innerHeight); // the floor of the map is 128 px from the bottom of the canvas
    const accg = 9.8; // 9.8 meters per second per second
    const playerDamageSpeed = 8; // if the player impacts another object at this velociyt then reigster fall damage 
    // canvas height is defined based on the w which is the window height
    canvas.height = Math.floor(0.9*w.innerHeight);
    canvas.width = Math.floor(0.9*w.innerWidth);
    game.lastTime = Date.now();
    game.entities = {}; // object for all the stuff currnetly on the screen
    game.frameCount = 0;
    game.startTime = Date.now();

    var Input = function(){
        this.keyPressCount = 0;
        this.lastKey = null; // bad I know...
        this.keyStatus = []; // array to store which key was pressed last 
        this.hi = "hello!";
    }

    // attach call backs to key codes
    Input.prototype.initKeys = function(){
        var inputThis = this;

        function lastKey(event,status){
            inputThis.keyPressCount ++;
            inputThis.lastKey = event.key;
            inputThis.keyStatus[event.key] = status;          
        }

        d.addEventListener('keydown',function(e){
            lastKey(e,true);
        });

        d.addEventListener('keyup',function(e){
            lastKey(e,false);
        });
    }

    // tell if all keys are up
    Input.prototype.allKeysUp = function(){

        var keysUp = true;

        for(var k in this.keyStatus){
            if(this.keyStatus[k]){
                keysUp = false; 
            }
        }

        return keysUp;
    }

    // object for the player ///////////////////////////////////////////////////////////////////////////////////////////
    var Player = function(){
        this.ds = 170; // speed of the player in pixles/second
        this.posX = 30;
        this.posY = 30;
        this._playerState = 1; // 0: idle, 1:running 2: walking
        this.entity = null; // entity is null, just draw a black dot for the player
        // make an enum for the player direction
        // these values where just found through trial and error, they're the indexes of the directions in the sprite sheet
        this.playerDir = {
          Up: 2,
          Down: 0,
          Left: 3,
          Right: 1
        }
  
        this._direction = this.playerDir.Down; // defult starting direction       
    }

    Player.prototype.drawPlayer = function(){
        if(this.entity == null){
            ctx.fillRect(this.posX,this.posY,10,10);
        } else {
            //debugger;  
            // posX,posY,view,angle,ctx
            //this.entity.renderSprite(this._playerState,this._direction,this.posX,this.posY); 
            this.entity.renderSprite(this._playerState,this._direction);
        }
     
    }

    // functions for moving the player around the map
    Player.prototype.moveUp = function(dt){
      this.posY = this.posY - this.ds*dt;
      this._direction = this.playerDir.Up;
    }

    Player.prototype.moveDown = function(dt){
      this.posY = this.posY + this.ds*dt;
      this._direction = this.playerDir.Down;
    }

    Player.prototype.moveLeft = function(dt){
      this.posX = this.posX - this.ds*dt;
      this._direction = this.playerDir.Left;
    }

    Player.prototype.moveRight = function(dt){
      this.posX = this.posX + this.ds*dt;
      this._direction = this.playerDir.Right;
    }
    
    
    // update the states of everything
    game.update = function(dt){
        if(game._player.entity == null) { // if there is no entity defined
            if(game._input.keyStatus['d']){
                //game._player.posX = game._player.posX + game._player.ds*dt; 
                game._player.moveRight(dt);
                console.log("right");
            }
            if(game._input.keyStatus['a']){
                //game._player.posX = game._player.posX - game._player.ds*dt;
                game._player.moveLeft(dt);
                console.log("left");
            }
            if(game._input.keyStatus['w']){
                //game._player.posY = game._player.posY - game._player.ds*dt;
                game._player.moveUp(dt);
                console.log("up");
            }
            if(game._input.keyStatus['s']){
                //game._player.posY = game._player.posY + game._player.ds*dt;
                game._player.moveDown(dt);
                console.log("down");
            }
        } else { // else if there's an entity defined 
            if(game._input.keyStatus['d']){ // right
                console.log("entity: right");
                game._player.entity.moveRight(dt);
                game._player._direction = game._player.playerDir.Right;
            }
            if(game._input.keyStatus['a']){ // left
                console.log("entity: left");
                game._player.entity.moveLeft(dt);
                game._player._direction = game._player.playerDir.Left;
            }
            if(game._input.keyStatus['w']){ // up
                console.log("entity: up");
                //game._player.entity.moveUp(dt);
                game._player._direction = game._player.playerDir.Up;
            }
            if(game._input.keyStatus['s']){ // down
                console.log("entity: down");
                //game._player.entity.moveDown(dt);
                game._player._direction = game._player.playerDir.Down;
            }
        }
        // update the player at a new location
        //debugger;
        this._player.entity.update(dt); 
        // see if the player object should be running or not
        if(game._input.allKeysUp()){ // check to see if all the keys are up, this assumes that _input is defined
            game._player._playerState = 0; // idle state
        } else {
            game._player._playerState = 1; // running state
        }
    }


        
//////////////////////////////////////////////////////////////////////////
    // object for handling the loading of files
    var Loader = function() {
        this.imgCache = [];
        this.loading = [];
        //this.readyCallBacks = []; // may not needed
        this.imgLoaded = []; // false until image is loaded
        this.readCallbacks = []; // callbacks for when everything is loaded; may not be used
        this.allLoadedCb = function() {Console.log("something is wrong here :)")}; // callback for when everything is loaded
    }

		// returns true if all images have finished loading.
		Loader.prototype.allLoaded = function(){
      var allLoaded = true;

      for(var k in this.imgCache){
        if(this.imgCache.hasOwnProperty(k) && !this.imgCache[k]){
          allLoaded = false;
        }
      }

      return allLoaded;
	}

    
    Loader.prototype.load = function(uriOrArr) {
      var loaderThis = this;
      if(uriOrArr instanceof Array) {
        // if array than iterate through the array and load everything in the array
        uriOrArr.forEach(function(elem){
          loaderThis.loadFile(elem);
        });
      } else { // if it's just a string then load that one string
        loaderThis.loadFile(uriOrArr);
      }
    }

    // loads a file from a uri/url
    // this is kind of the meat of loading files
    Loader.prototype.loadFile = function(fileName) {
      var loaderThis = this; // ref to the this in qestion
      if(this.imgCache[fileName]){ // if the file is loaded then just return it 
        return this.imgCache[fileName];
      } else { // if it is not loaded then load it 
        var imgsp = new Image();
        imgsp.onload = function(){
          loaderThis.imgCache[fileName] = imgsp; // image is now loaded, save it to the image cache
          // check to see if all the images have been loaded.
          // if yes then call the all loaded callback
          if(loaderThis.allLoaded()){
            loaderThis.allLoadedCb();
          }
        }
        // defult values while the system is loaded
        this.imgCache[fileName] = false;
        imgsp.src = fileName;
      }
    }

    // factory function in loader for making sprites.  
    Loader.prototype.getSprites = function(){
      // right now just use the constents for the dt sprite sheet
      var SheetWidth = 1536;
      var SheetHeight = 1024;
      var SpriteWidth = 256;
      var SpriteHeight = 256;
      var AnimationFrames = 10;
      var AngularViews = 4;
      // sizes of the sprite to be rendered 
      var spSizeX = 128;
      var spSizeY = 128;
      var renderSpeed = 9;
      // create an array and and the sprites to it
      var sprites = [];
      for(var k in this.imgCache){
        // load the sprite sheet image into a sprite sheet object 
        var newSprite = new Sprite(SheetWidth,SheetHeight,SpriteWidth,SpriteHeight,AnimationFrames,AngularViews,spSizeX,spSizeY,renderSpeed,this.imgCache[k]);
        sprites.push(newSprite);
      }

      return sprites;
    }

    
    // callback for when allspries are called
    Loader.prototype.onAllLoaded = function(callback){
      this.allLoadedCb = callback;
    }

    // loads and creates sprite objects based on json data
    // takes in an array of strings which are the file names of all the json files that have sprite info
    Loader.prototype.loadSpriteFromJson = function(jsonArr) {

      // nested function to use as a call back for loading sprites 
      function onJsonLoad()
      {
        if(this.status == 200){
          var loadedJson = this.responseText;
        }
      }      

      for(var index in jsonArr){
        var oReq = new XMLHttpRequest();
        oReq.addEventListener("load", onJsonLoad);
        var path = jsonArr[index];
        oReq.open("GET", jsonArr[index],false);
        oReq.send();
      }
    }

    // function for loading in a list of sprite sheets with the same demintions
    // filelist is a list of files
    // listDims is the list of demintions 
    // callback is a call back function that is called when stuff is finished loading
    // sheetWidth,sheetHight,keyWidth,keyHeight,keys,views,rSizeX,rSizeY,spSizeX,spSizY
    Loader.prototype.loadSpriteSheet = function(SheetWidth,SheetHeight,SpriteWidth,SpriteHeight,AnimationFrames,AngularViews,spSizeX,spSizeY,renderSpeed,sheetDims) {
      var sprites = [];

      if(AnimationFrames instanceof Array){ // test to see if there's more than one type of animation frame
        //var frameCount = AnimationFrames.length; // number of animation frames in the sprite sheet 
        var frameIndex = 0;
        for(var k in this.imgCache){
            // get data for thing
            var imgData = this.imgCache[k];
            var frames = AnimationFrames[frameIndex];
            frameIndex++;
            // now make a new sprite and add it to the thing
            var newSprite = new Sprite(SheetWidth,SheetHeight,SpriteWidth,SpriteHeight,frames,AngularViews,spSizeX,spSizeY,renderSpeed,imgData);
            sprites.push(newSprite); 
        }        

      } else {
      if(!sheetDims){
            for(var k in this.imgCache){
                var newSprite = new Sprite(SheetWidth,SheetHeight,SpriteWidth,SpriteHeight,AnimationFrames,AngularViews,spSizeX,spSizeY,renderSpeed,this.imgCache[k]);
                sprites.push(newSprite);
            } 
        } else { // if the sheet dims array is defined
        
        }
      }
    
      return sprites;
    }

/////////////////////////////
    // object for handling sprite sheets
    // this is kinda complex, but one of the cooler parts of this program
    // a sprite is defined as a single sprite sheet. 
    var Sprite = function(sheetWidth,sheetHight,keyWidth,keyHeight,keys,views,rSizeX,rSizeY,updateSpeed,sheetImg){
      this._sheetImg = sheetImg;
      this.sheetWidth = sheetWidth; // width of the sprite sheet
      this.sheetHeight = sheetHight; // height of the sprite sheet
      // height and width of indivual keys in the sprite sheet
      this.keyWidth = keyWidth; 
      this.keyHight = keyHeight;
      // set the sizes for the x and y of the sprites
      this._renderSizeX = rSizeX;
      this._renderSizeY = rSizeY;
      // set some other stuff for the keys
      this._keys = keys; // number of keys in each frame
      this.views = views; // number of views that the sprite can have 
      this._index = 0; // internal index for which frame is being shown 
      this._renderSpeed = updateSpeed; // right now defult render speed of the sprite
    }

    // update drawing index aginst the current time
    Sprite.prototype.update = function(dt){
      this._index += this._renderSpeed*dt;
    }

    Sprite.prototype.render = function(ctx,viewAngle,posX,posY){
      if(this._renderSpeed > 0){
        var idx = Math.floor(this._index) % this._keys;
        //console.log("index: "+idx);
        // use the local render key function to draw the sprite in qestion
        this.renderKey(posX,posY,idx,viewAngle);
      }
    }

    // simple function for rendering a single key of the sprite sheet 
    // also sort of used to figure out how canvas renders images
    // https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Using_images#Slicing
    Sprite.prototype.renderKey = function(posX,posY,keyFrame,view){
      var sx = this.keyWidth * keyFrame; // figure out which frame of the view to show
      var sy = this.keyHight * view; // figure out which view to show
      ctx.drawImage(this._sheetImg,sx,sy,this.keyWidth,this.keyHight,posX,posY,this._renderSizeX,this._renderSizeY);
    } 

    // define a function for an entity
    // an entity has mass, several views represented by diffrent sprites, and a refrence to a keyboard or other input device
    var Entity = function(frames,mass,hasMass,deltaS,pX,pY){
        this._viewCount = 0;
        this._frames = frames;
        this._mass = mass; // character has some mass
        this._hasMass = hasMass; // do we want to apply mass to this entity?
        this._currentView = 0; // current view of the thing 
        // velocity and time for doing falling calculations
        this._fallingVelocity = 0; // inital velcity and intigrand when figuring acc due to g if entity has mass 
        this._fallTime = 0; // time that we're falling for

        // currentPosX and currentPosY are no longer needed
        //this._currentPosX = 0;
        //this._currentPosY = 0;
        this.ds = deltaS; // speed of the player
        this.gravity = 9.8; // acceration dude to gravity
        // enity x and y for position
        if(!pX || !pY){
            this._posX = null;
            this._posY = null;
        } else {
            this._posX = pX;
            this._posY = pY;
        }
    
        if(frames instanceof Array){ // check if there's multible views
            this._viewCount = frames.length;
        } else {
            this._viewCount = 1;
        }

        // every time a direction if an entiy is changed make sure that we have call backs
        this._directions = []; 

    }

    // set the callbacks for the directions
    Entity.prototype.setDirectionCB = function(direct,callbackDR){
        this._directions[direct] = callbackDR; 
    }

    // set the inital value for position
    Entity.prototype.setInitalPos = function(px,py){
        if(this._posX == null && this._posY == null){
            this._posX = px;
            this._posY = py;
        }
    }

    // functions for moving the entity around the screen
    Entity.prototype.moveUp = function(dt){
        this._posY = this._posY - this.ds*dt;
        if(this._directions['up']){

        }
    }

    Entity.prototype.moveDown = function(dt){
        this._posY = this._posY + this.ds*dt;
        if(this._directions['down']){

        }
    }

    Entity.prototype.moveLeft = function(dt){
        this._posX = this._posX - this.ds*dt;
        if(this._directions['left']){

        }
    }

    Entity.prototype.moveRight = function(dt){
        this._posX = this._posX + this.ds*dt;
        if(this._directions['right']){

        }
    }

    // Enity method for rendering an entity
    // takes in an enity then renders it.
    Entity.prototype.renderSprite = function(view,angle,posX,posY){ // defult to local if posX and posY are undefined
        //debugger;
        this._currentView = view; // update the current view, this is very important for the update method 
        var currentSprite = this._frames[view]; // pull the current view, running , idle or walking 
        // update the current posX and posY
        if(posX && posY){ 
            currentSprite.render(ctx,angle,posX,posY);  
        }else{
            currentSprite.render(ctx,angle,this._posX,this._posY);
        }
    }

    // update method, take in dt and then figure out the current sprite, then call sprite update 
    Entity.prototype.update = function(dt){
        // account for acc
        if(this._hasMass){
            // intigrate t and g to find out dy
            var dy = 0;
            if(this._posY < roomFloor){
                this._fallTime += dt; // basicly a definite inigral
                dy = 0.5*accg*Math.pow(this._fallTime,2); 
                this._posY += dy;
            } else { 
                // if we're outside of the range of we're falling in, then zero everything out
                this._fallTime = 0;
                dy = 0;          
            }
            // figure out if dy is above hit damage and figure out if the player is touching anything
      
        }

        var currentSprite = this._frames[this._currentView]; // which trump are we looking at
        currentSprite.update(dt); // just call that sprites update, nothing special here
    }


    // main function
    game.main = function(){
        var dt = (Date.now() - game.lastTime)/1000.0;
        //fps = game.frameCount / dt;
        ctx.clearRect(0,0,canvas.width,canvas.height); // clear the screen for the new frame
        
        // draw the FPS
        //ctx.font = '40px serif';
        //ctx.fillText('fps: '+fps, 10, 50);
        game._player.drawPlayer();
        game.update(dt);
        
        game.lastTime = Date.now();
        game.frameCount += 1;
        //////////////////////////////////////////
        // do a simple render of the first frame of the idle sprite sheet
        //var sp1 = game.entities[1]; // idleing trump sprite
        //sp1.renderKey(40,40,0,0);
        //////////////////////////////////////////
        //sp1.render(ctx,1,45,30);
        // update all the graphics on the screen
        //sp1.update(dt);

        w.requestAnimationFrame(game.main);
    }

    // start up functions for the game
    // start up game objects
    game._input = new Input();
    game._player = new Player(); 
    game._loader = new Loader();
    // attach a call to the input object 
   //debugger;
    w.onload = function(){
        game._input.initKeys();
      
        
        game._loader.load(['./sprites/trump_iddle.png',
                           './sprites/trump_run.png',
                           './sprites/trump_walk.png'
                          ]);
        
        // call to method for loading all sprites from json
//        game._loader.loadSpriteFromJson([
//          "./sprites/trump_iddle.json",
//          "./sprites/trump_run.json",
//          "./sprites/trump_walk.json"
//        ]);

          // callback for when all the assets for the game are loaded
          game._loader.onAllLoaded(function(){
            //game.entities = game._loader.getSprites(); // factory method for returning a list of sprites 
            // download sprites for all the directions
            // all the sprites for the player
            // right now just use the constents for the dt sprite sheet
            var SheetWidth = 1536;
            var SheetHeight = 1024;
            // key width and height
            var SpriteWidth = 256;
            var SpriteHeight = 256;
            var AnimationFrames = 6;
            var AngularViews = 4;
            // sizes of the sprite to be rendered on the screen
            var spSizeX = 128;
            var spSizeY = 128;
            var renderSpeed = 12;
            var frames = [10,6,10]; // number of frames, asinged in order of when they're loaded
            game.entities = game._loader.loadSpriteSheet(SheetWidth,SheetHeight,SpriteWidth,SpriteHeight,AnimationFrames,AngularViews,spSizeX,spSizeY,renderSpeed);
            var spriteSheets = game._loader.loadSpriteSheet(SheetWidth,SheetHeight,SpriteWidth,SpriteHeight,frames,AngularViews,spSizeX,spSizeY,renderSpeed);
            var trumpSprites = spriteSheets.slice(0,3); // donald is the first 3 sprites loaded
            var entityTrump = new Entity(trumpSprites,120,true,170,30,30); // make an trump enity, with 120 kG of mass
            // attach the trump entity to the player object
            game._player.entity = entityTrump;

            game.main(); // and finily start the main game loop              
        });
    }

    
   

})(window,document);
