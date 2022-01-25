//Do no modify this class, it is tightly bound to the game engine
foreign class Shader {
	construct newByID(id){}        //Creates a new shader given its id
	construct newByName(name){}    //Creates a new shader given its name
	foreign initUniform(name){}		 //Initialize an uniform given its name (the id is given by order of initialization)
	foreign initAttribute(name, ){} 
  foreign getUniform(id)         //Gets the uniform n° id
	foreign setUniform(id, value)  //Sets the value of the uniform n° id
}
