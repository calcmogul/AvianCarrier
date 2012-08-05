//=============================================================================
//File Name: setParameter.inl
//Description: Holds definition for template function "setShaderParam" from
//             InfoWindow class
//Author: Tyler Veness
//=============================================================================

#ifndef SETPARAMETER_INL_
#define SETPARAMETER_INL_

template<class T> void InfoWindow::setShaderParam( const std::string& name , T var ) {
	if ( shaderState.shader != NULL )
		shader.setParameter( name , var );
}

#endif /* SETPARAMETER_INL_ */
