<?php

function applications_list($arg){


	$header = array(
	 'application' => array('data' => t('Aplicacion'), 'field' => 'a.application','sort' => 'desc'),
	 'description' => array('data' => t('Descripcion'), 'field' => 'a.description'),
	 'status' => array('data' => t('Status'), 'field' => 'a.status'),
	 'operations' => array('data' => t('Operations')),
	);
	
	$qry = db_select('sso_cat_applications','a');
	user_build_filter_query($qry);
	
  $count_query = clone $qry;
  $count_query->addExpression('COUNT(a.id_application)');

  $qry = $qry->extend('PagerDefault')->extend('TableSort');
  $qry
    ->fields('a', array('id_application', 'application', 'description', 'status'))
    ->limit(10)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $qry->execute();	

  $destination = drupal_get_destination();
  $status = array(t('inactivo'), t('activo'));
  
  foreach ($result as $apps) {

  	$options[$apps->id_application] = array(
		'application'=>$apps->application,
		'description'=>$apps->description,
		'status'=>$status[$apps->status],
		'operations' => array('data' => array('#type' => 'link', '#title' => t('editar'), '#href' => "admin/config/sso/aplicaciones/edit/$apps->id_application")),
	);
      
  }
  	$form['add']=array(
			'data' => array('#type' => 'link', '#title' => t('Nueva Aplicacion'), '#href' => "admin/config/sso/aplicaciones/create")
	);
  $form['applications'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('NO hay aplicaciones activas.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));


  return $form; 
}

function comnunication_list(){
	
	$header = array(
	 'communication' => array('data' => t('Comunicacion'), 'field' => 'c.communication','sort' => 'desc'),
	 'status' => array('data' => t('Status'), 'field' => 'c.status'),
	 'operations' => array('data' => t('Operations')),
	);
	
	$qry = db_select('sso_cat_type_communication','c');
	user_build_filter_query($qry);
	
  $count_query = clone $qry;
  $count_query->addExpression('COUNT(c.id_cat_type_communication)');

  $qry = $qry->extend('PagerDefault')->extend('TableSort');
  $qry
    ->fields('c', array('id_cat_type_communication', 'communication', 'status'))
    ->limit(10)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $qry->execute();	
  
    $destination = drupal_get_destination();
  $status = array(t('inactivo'), t('activo'));	
  
    foreach ($result as $communication) {

  	$options[$communication->id_cat_type_communication] = array(
		'communication'=>$communication->communication,
		'status'=>$status[$communication->status],
		'operations' => array('data' => array('#type' => 'link', '#title' => t('editar'), '#href' => "admin/config/sso/comunicacion/edit/".$communication->id_cat_type_communication)),
	);
	}

  	$form['add']=array(
			'data' => array('#type' => 'link', '#title' => t('Nueva comunicacion'), '#href' => "admin/config/sso/comunicacion/create")
	);
  $form['applications'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('NO hay comunicaciones activas.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));


  return $form; 	
}


function webservice_list(){
	
	$header = array(
	 'url' => array('data' => t('URL'), 'field' => 'w.url','sort' => 'desc'),
	 'application'=>array('data' => t('Aplicacion'), 'field' => 'a.application'),
	 'communication'=>array('data' => t('Tipo de Comunicacion'), 'field' => 'co.communication'),
	 'status' => array('data' => t('Status'), 'field' => 'w.status'),
	 'operations' => array('data' => t('Operations')),
	);
	
	$options=array();
	
	$qry = db_select('sso_webservers','w')
    ->fields('w', array('id_webserver', 'url', 'status'))
	->fields('a',array('application'))
	->fields('co',array('communication'));
	$qry->join('sso_cat_applications', 'a', 'a.id_application = w.id_application');
	$qry->join('sso_cat_type_communication','co','co.id_cat_type_communication = w.id_cat_type_communication');

	user_build_filter_query($qry);
	
  $count_query = clone $qry;
  $count_query->addExpression('COUNT(w.id_webserver)');

  $qry = $qry->extend('PagerDefault')->extend('TableSort');
  $qry
    ->fields('w', array('id_webserver', 'url', 'status'))
	->fields('a',array('application'))
	->fields('co',array('communication'))
    ->limit(10)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $qry->execute();	
  
    $destination = drupal_get_destination();
  $status = array(t('inactivo'), t('activo'));	
  
    foreach ($result as $webservers) {

  	$options[$webservers->id_webserver] = array(
  		'url'=>$webservers->url,
  		'application'=>$webservers->application,
		'communication'=>$webservers->communication,
		'status'=>$status[$webservers->status],
		'operations' => array('data' => array('#type' => 'link', '#title' => t('editar'), '#href' => "admin/config/sso/webservice/edit/".$webservers->id_webserver)),
	);
	}

  	$form['add']=array(
			'data' => array('#type' => 'link', '#title' => t('Nuevo WebService'), '#href' => "admin/config/sso/webservice/create")
	);
  $form['applications'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('NO hay WebServices configurados.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));


  return $form; 	
}