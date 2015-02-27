<?php

 
 /**@brief 
  * funcion que genera el formulario de busqueda sobre las bitacoras con diferemtes filtros 

  * @author Miguel Angel Chávez Obregón
  * @date 2014-10-10
  * 
 */
function bitacora_sso_form_reportes($form, &$form_state, $args = 'NULL'){

    $form['description']=array(
    '#type' => 'item',
     );
    
	$form['search']= array(
	'#type' => 'fieldset', 
    '#title' => t('Buscar'),
	);
	
    $form['search']['usuarios'] = array(
     '#type' => 'select',
  	 '#title' => t('Usuario'),
  	 '#options' => bitacora_sso_cat_user(),        
    );
    
    $form['search']['roles'] = array(
     '#type' => 'select',
  	 '#title' => t('Rol'),
  	 '#options' => bitacora_sso_cat_rol(),        
    );	
	
	  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => 'Buscar',
  );
    return $form; 
    
}
