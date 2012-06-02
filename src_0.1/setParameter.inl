//=============================================================================
//File Name: setParameter.inl
//Description: Holds definition for template function "setParameter" from
//             InfoWindow class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef SETPARAMETER_INL_
#define SETPARAMETER_INL_

template<class T> void InfoWindow::setShaderParam( const std::string& name , T var ) {
	if ( shaderState.shader != NULL )
		shader.setParameter( name , var );
}

#endif /* SETPARAMETER_INL_ */
