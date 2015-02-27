<?php


/** @brief form_carga_group_edit()
 * formulario Para edición de usuario.
 * 
 * @author Sajit Avila
 * @date 2014-12-03
*/

function form_carga_group_edit($form, &$form_state, $args=NULL){
	
	global $user;
	$group = group_sso_get($args);
	$niveles = get_levelsAcademy($user->idaccount);
	$niveles = explode(',',$niveles[0]->level);
	$count = 1;
	foreach($niveles as $n){
		$levels[$count] = $n;
		$count++;
	}
	
	$grados = grados_sso_catalogo($group[0]->level);
	$form['description']=array(
	    '#type' => 'item',
	    '#title' => '',	
	);
	
	$form['group']['niveles']=array(
	  	'#type' => 'select',
	  	'#title' => t('Niveles'),
	  	'#options' => $levels,
	  	'#default_value' => $group[0]->level,
	  	'#id'=>'levels',	
	);
	
	$form['group']['grados']=array(
	  	'#type' => 'select',
	  	'#title' => t('Grados'),
	  	'#options' => $grados,
	  	'#default_value' => $group[0]->grade,
	);
	
	$form['group']['gid']=array(
	  	'#type' => 'hidden',
	  	'#value' => $args,
	);
	
	$form['group']['name']=array(
		'#type' => 'textfield',
	    '#title' => t('Nombre del Grupo'),
		'#required' => TRUE,
	    '#description' => "Ingresa el nombre del grupo (ej.A,B,C)",
	    '#default_value' => $group[0]->groups
	);
	
  	$form['submit']=array(
	    '#type' => 'submit',
	    '#value' => 'Actualiza',  
  	);	
		
	return $form;
		
}


/** @brief form_carga_group_edit_submit()
 * funcion para procesar el formulario de edicion de usuario.
 * 
 * @author Sajit Avila
 * @date 2014-12-03
*/


function form_carga_group_edit_submit($form, &$form_state){
	$name = $form_state['name'];
	$gid = $form_state['gid'];
	$niveles = $form_state['niveles'];
	$grados = $form_state['grados']+1;
	$fields = array(
	    	'groups'=>$name,
	    	'level'=> $niveles,
	    	'grade' => $grados,
		);
	
	$result = db_update('sso_groups')
	->fields($fields)
	->condition('gid',$gid)
	->execute();
	
	return $result;
}
/** @brief form_carga_group_new()
 * funcion para crear un nuevo usuario
 * 
 * @author Sajit Avila
 * @date 2014-12-03
*/

function form_carga_group_new($form, &$form_state, $args=NULL){
	
	$escuelas = escuelas_sso_catalogo();
	$niveles = niveles_sso_catalogo();
	$grados = grados_sso_catalogo();
	
	$form['description']=array(
    '#type' => 'item',
    '#title' => '',	
	);
	
	$form['group'] = array(
	   '#type' => 'fieldset', 
	   '#title' => t('Usuario'),
	   '#collapsible' => TRUE, // Added 
	   '#collapsed' => FALSE, // Added
	);
	
	$form['group']['escuelas']=array(
	  	'#type' => 'select',
	  	'#title' => t('Escuelas'),
	  	'#options' => $escuelas,
	);
	
	$form['group']['niveles']=array(
	  	'#type' => 'select',
	  	'#title' => t('Niveles'),
	  	'#options' => $niveles,
	);
	
	$form['group']['grados']=array(
	  	'#type' => 'select',
	  	'#title' => t('Grados'),
	  	'#options' => $grados,
	);
	
	$form['group']['name']=array(
		'#type' => 'textfield',
	    '#title' => t('Nombre del Grupo'),
		'#required' => TRUE,
	    '#description' => "Ingresa el nombre del grupo (ej.A,B,C)",
	);
	
  $form['submit']=array(
    '#type' => 'submit',
    '#value' => 'Genera',  
  );		
	return $form;
}

/** @brief form_carga_group_new_submit()
 * funcion para procesar informacion y agregar el usuario.
 * 
 * @author Sajit Avila
 * @date 2014-12-03
*/


function form_carga_group_new_submit($form, &$form_state){
	global $user;
	$group = $user->idaccount;
	$name = $form_state['name'];
	$niveles = $form_state['level'];
	$grados = $form_state['grade'];
	$fields = array(
	    	'groups'=>$name,
	    	'college'=> $group,
	    	'level'=> $niveles,
	    	'grade' => $grados,
	    	'active' => 1
		);
	$result = db_insert('sso_groups')
	->fields($fields)
	->execute();
	return $result;
}
