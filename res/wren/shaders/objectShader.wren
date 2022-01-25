import "shader" for Shader
/* The line above imports the Shader class.
 * You can then instantiate that class by using Shader.newByID(id) or Shader.newByName(name).
 * Then, using shader.setUniform(id) and shader.getUniform(id) you can modify the behavior of the shader
 */

//In here, you have to instantiate the Shader class and return the object
init(){
	return Shader.newByID(0);
}

//This is the entry point, write your code in here, it'll get executed once per frame
update(shader){
	
}
