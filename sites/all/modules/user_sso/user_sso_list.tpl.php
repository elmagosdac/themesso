<?php //print drupal_render($form);
global $base_url
?>
<!-- DataTables CSS -->
<link rel="stylesheet" type="text/css" href="http://cdn.datatables.net/1.10.4/css/jquery.dataTables.css">

<!-- jQuery -->
<script type="text/javascript" charset="utf8" src="//code.jquery.com/jquery-1.11.2.min.js"></script>
<script src="<?php echo $base_url?>/sites/all/modules/user_sso/js/jquery-migrate-1.2.1.min.js"></script>

<!-- DataTables -->
<script type="text/javascript" charset="utf8" src="http://cdn.datatables.net/1.10.4/js/jquery.dataTables.js"></script>

<script type="text/javascript" charset="utf8">


	$(document).ready( function () {
	    $('#listaUser').DataTable(
		{
			"jQueryUI": true,
			"info": false,
			"processing": true,
			columns:[
				{data:'Editar'},
				{data:'Borrar'},
				{data:'Nombre'},
				{data:'Email'},
			]
		 }
	    );    
} );
	

	$(function(){
		//js actions button menu
		$('.list').click(function(){
			$('.fram').stop().slideUp(300,function(){
				$('.rows.fram').stop().slideDown(300);
			});
		});
		
		$('.new').click(function(){
			$('.fram').stop().slideUp(300,function(){
				$('.add_new.fram').stop().slideDown(300);
			});
		});
		
		$('.mass').click(function(){
			$('.fram').stop().slideUp(300,function(){
				$('.massive.fram').stop().slideDown(300);
			});
		});
		
		$('.find').click(function(){
			$('.fram').stop().slideUp(300,function(){
				$('.busqueda.fram').stop().slideDown(300);
			});
		});
	});
</script>


<div class="content_list">
	<div class="tool_list">
		<button class="list"> Listado </button>
		<button class="new"> Nuevo Usuario </button>
		<!-- <a href="user-sso/new">Nuevo Usuario</a> -->
		<button class="mass"> Carga Masiva </button>
		<!-- <a href="user-sso/carga-masiva">Carga Masiva</a> -->
	</div>
	
	<div class="frames">
		<div class="layer" style=" display:none"><div class="mensaje"></div></div>
		<div class="rows fram">
			<table id="listaUser"  class="display">
				<thead>
					<tr>
						<th>Editar</th>
						<th>Borrar</th>
						<th>Nombre</th>
						<th>Email</th>
					</tr>
				</thead>
				<tbody>
				<?php $i=0; foreach($usuarios['usuarios_lista'] as $g):?>
					<tr>
						<td class="fila_<?php echo $i%2?>"><a href="user-sso/edit/<?php echo $g->uid?>">Edit</button></td>
						<td class="fila_<?php echo $i%2?>"><a href="<?php echo $g->uid?>" class="del">del</button></td>
						<td class="fila_<?php echo $i%2?>"><?php echo $g->username?></td>
						<td class="fila_<?php echo $i%2?>"><?php echo $g->mail?></td>
					</tr>
				<?php $i++; endforeach;?>
				</tbody>
			</table>
			<!--fdsffsdfs
			<?php //echo $usuarios['paginador']?>
			<style>
				.paginador {
				    list-style: none outside none;
				    margin: 0 !important;
				    padding: 0 !important;
				}
				.paginador li {
				    float: left;
				    margin: 0 2px;
				    text-align: center;
				}
				.pagina {
				    background-color: rgba(0, 0, 0, 0.4);
				    border-radius: 50%;
				    color: #fff;
				    float: left;
				    font-family: sans-serif;
				    font-weight: bold;
				    height: 30px;
				    line-height: 30px;
				    width: 30px;
				}
				
				.pagina:hover{
					background-color:rgba(0,0,0,0.2);
					text-decoration: none;
				}
				
				.pagina.current{
					background-color:#1b1b1b;
				}
				
				.prev,.next{
					background-color: rgba(0, 0, 0, 0.4);
				    border-radius: 10px;
				    color: #fff;
				    float: left;
				    font-family: sans-serif;
				    font-weight: bold;
				    height: 30px;
				    line-height: 30px;
				    padding:0 3px;

				}
				
			</style>
			<script>
				$(function(){
					$('tr').hover(function(){
						$(this).children($('td')).addClass('hover');
					},function(){
						$(this).children($('td')).removeClass('hover');
					});
					
					$('.del').click(function(e){
						e.preventDefault();
						$.ajax({
							url:'user-sso/del/'+$(this).attr('href'),
							success:function(dataR){
								console.log(dataR);
							}
						});
						
					})
				});
			</script>
			<style>
			
			.fila_1 {
			    background-color: #ccc;
			}
			
			
			.hover{
			    background-color: #aaaaaa;
			    cursor:pointer;
			}
			
			</style>-->
		</div>
		<div class="add_new fram"style="display:none">
			<script>
				$(function(){
					$('input[type=checkbox]').removeAttr('checked');
					function validarEmail( email ) {
					    expr = /^([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$/;
					    if(!expr.test(email)){
					        return false;
					    }else{
					    	return true;
					    }
					}
					
					$('#edit-mail').bind('keyup',function(){
						var res = validarEmail($(this).val());
						if(res){
							$(this).css({'box-shadow':'0 0 4px -2px green'});
						}else{
							$(this).css({'box-shadow':'0 0 4px -2px red'});
						}
					});
					
					$('#edit-mail').bind('blur',function(){
						var campo = $(this);
						if(validarEmail(campo.val())){
							$.ajax({
								url:'user-sso/validarmail',
								type:'POST',
								data:{mail:''+campo.val()},
								dataType:'json',
								success:function(resultVal){
									if(resultVal.result == 300){
										$(this).css({'box-shadow':'0 0 4px -2px red'});
										$('.buton_content').hide();
										$('.errors').html('correo invalido, coloque un email real');
										$('.errors').fadeIn(300);
									}else{
										$('.buton_content').show();
										$(this).css({'box-shadow':'0 0 4px -2px green'});
										$('.errors').fadeOut(300);
										$('.errors').empty();
									}
								}
							});
						}
					});
					
					function studenCheck(){
						if($('input[value=4]').attr('checked') == 'checked'){
							$('.apps, .parent').stop().fadeIn(300);
							$('.apps input').each(function(){
								if($(this).val() == 1 || $(this).val() == 2){
									$('.apps input[value='+$(this).val()+'], .apps span[value='+$(this).val()+']').show();
								}else{
									$('.apps input[value='+$(this).val()+'], .apps span[value='+$(this).val()+']').hide();
								}
							});
						}else if($("input[value=3]").attr('checked') == 'checked' || $("input[value=7]").attr('checked') == 'checked' || $("input[value=6]").attr('checked') == 'checked' || $("input[value=5]").attr('checked') == 'checked'){
							$('.parent').stop().fadeOut(300);
							$('.apps').stop().fadeIn(300);
							$('.apps input, .apps span').stop().fadeIn(300)	
						}else{
							$('.apps,.parent').hide();
						}
					}
					
					//accion radio
					$('input[type=radio]').bind('change',function(){
						if($(this).attr('checked') == 'checked' && $(this).attr('value') == '4'){
							$('.parent').fadeIn(400);
						}else{
							$('.parent').fadeOut(400);
						}
						
						if($(this).attr('checked') == 'checked' && $(this).attr('value') == '6'){
							$('.parent').fadeOut(400);
							$('.input_ce').fadeIn(400);
						}else{
							$('.input_ce').fadeOut(400);
							$('.user_data').fadeIn(400);
							$('.buton_content').fadeIn(300);
						}
					});
					
					$('input[value=4]').bind('change',function(){
						studenCheck();
					});
					
					$('input[value=6]').bind('change',function(){
						if($(this).attr('checked') == 'checked'){
							$('.user_data').slideUp(300,function(){
								$('.input_ce').fadeIn(300);
							});
							$('.buton_content').fadeOut(300);
							
						}else{
							$('.user_data').fadeIn(300);
							$('.buton_content').fadeIn(300);
							
							$('.input_ce').slideUp(500,function(){
								$('.user_data').fadeIn(300);
								$('.escuela').html('');
							});
							$('input[type=text]').attr('value','');
						}
					});
					
					$('input[value=3], input[value=6], input[value=7], input[value=5]').bind('change',function(){
						if($(this).attr('checked') == 'checked'){
							$('.apps').stop().fadeIn(300);
							$('.apps input, .apps span').stop().fadeIn(300)
						}else if($("input[value=3]").attr('checked') == 'checked' || $("input[value=7]").attr('checked') == 'checked' || $("input[value=6]").attr('checked') == 'checked' || $("input[value=5]").attr('checked') == 'checked'){
							$('.apps').stop().fadeIn(300);
							$('.apps input, .apps span').stop().fadeIn(300)
						}else{
							$('.apps').stop().fadeOut(300);
							$('.apps input, .apps span').stop().fadeOut(300)
						}
					});
			
					studenCheck();
					
					$('#<?php echo $formnew['#id']?>').submit(function(e){
						e.preventDefault();
						var req = true;
						$('.required').each(function(){
							if($(this).val() == ''){
								console.log($(this).attr('name'));
								$(this).css({'box-shadow':'0 0 4px -2px red'});
								req = false;
							}else{
								$(this).css({'box-shadow':''});
							}
						});			
						if(req){
							
							$.ajax({
								type:'POST',
								url:'user-sso/new',
								data: $(this).serialize(),
								dataType:'json',
								success:function(dataBack){
									alert(dataBack.mensaje);
									var result;
									if(dataBack.code == 200 ){
										$('html,body').stop().animate({scrollTop:0},300);
										$('input[type=text]').attr('value','');
										$('input[type=checkbox]').removeAttr('disabled');
										$('input[type=checkbox]').removeAttr('checked');
										$('.escuela').empty();
									}
								}
							});
						}
					});
				});
			</script>
			<div class="cont_token_form ">
				<form action="<?php echo $formnew['#action'];?>" method="<?php echo $formnew['#method'];?>" id="<?php echo $formnew['#id'];?>">
				<div class="left_col">
					<div class="campos">
						<?php print drupal_render($formnew['user']['uid'])?>
					</div>
					<div class="campos role">
						<?php $listRol = $formnew['user']['role']['#value'];?>
						<?php foreach($listRol as $kr=>$lr){?>
							<div class="rols">
								<input  name="role[]" type="radio" value="<?php echo $kr?>">
								<span><?php echo $lr?></span>
							</div>
						<?php }?>
					</div>
					<div class="input_ce" style="display:none">
						Validacion de Licencia
						<input type="text" class="ce_val" placeholder="Licencia">
						<button class="send_ceVal">Validar</button>
						
					</div>
					
					<script>
						$(function(){
							$('.send_ceVal').click(function(e){
								e.preventDefault();
								$('.imgload').fadeIn(200);
								$.ajax({
									url:'user-sso/soapajax',
									type:'POST',
									data:{
										contactid: ''+$('.ce_val').val(),
										username: 'Denumeris',
										password: 'D3num3r1s',
										method:'GetContact'
									},
									dataType:'json',
									success:function(xmlResult){
										if(xmlResult.GetContactResult){
											var mobj = $.parseJSON(xmlResult.GetContactResult);
											var acccount = '';
											$.each(mobj,function(k,obj){
												console.log(obj);
												$('#edit-mail').attr('value',obj.email);
												$('#edit-username').attr('value',obj.firstname);
												$('#edit-lastname').attr('value',obj.lastname);
												account = obj.accountid;
												$('.license').attr('value',$('.ce_val').val());
											});
											
											$.ajax({
												url:'user-sso/soapajax',
												type:'POST',
												data:{
													accountid: account,
													username: 'Denumeris',
													password: 'D3num3r1s',
													method:'GetAccountInfo'
												},
												dataType:'json',
												success:function(datas){
													var mobjs = $.parseJSON(datas.GetAccountInfoResult);
													var tabHTML = '<table>';
													$.each(mobjs,function(ks,objs){
														console.log(objs);
														tabHTML +='<tr><td>'+objs.name+'</td></tr>';
														tabHTML +='<tr><td>'+objs.city+'</td></tr>';
														tabHTML +='<tr><td>'+objs.neighborhood+'</td></tr>';
														tabHTML +='<tr><td>'+objs.street+'</td></tr>';
													});
													tabHTML +='</table>';
													$('.escuela').html(tabHTML);
													
													$('.input_ce').slideUp(300,function(){
														$('.user_data').fadeIn(300);
														$('.imgload').fadeOut(200);
														$('.buton_content').fadeIn(300);
													});
												}
											});
										}else{
											alert(xmlResult.error);
										}
									}
								});
							});
						});
					</script>
					<div class="user_data">
						<!-- <div class="campos">
							<?php print drupal_render($form['user']['name'])?>
						</div> -->
						<input class="license" name="license" type="hidden">
						<div class="campos">
							<?php print drupal_render($formnew['user']['username'])?>
						</div>
						<div class="campos">
							<?php print drupal_render($formnew['user']['middlename'])?>
						</div>
						<div class="campos">
							<?php print drupal_render($formnew['user']['lastname'])?>
						</div>
						<div class="campos">
							<?php print drupal_render($formnew['user']['sex'])?>
						</div>
						<div class="campos">
							<?php print drupal_render($formnew['user']['matricula'])?>
						</div>
						<div class="campos">
							<?php print drupal_render($formnew['user']['mail'])?>
						</div>
						<div class="campos apps" style="display:none">
							<?php $listRol = $formnew['user']['apps']['#value'];?>
							<?php foreach($listRol as $kr=>$lr){?>
								<input style="display: none" name="apps[]" type="checkbox" value="<?php echo $kr?>">
								<span style="display: none"  value="<?php echo $kr?>"><?php echo $lr?></span>
							<?php }?>
						</div>
						<div class="campos">
							<?php print drupal_render($formnew['user']['level']);?>
						</div>
						<div class="campos">
							<?php print drupal_render($formnew['user']['grade']);?>
						</div>
						<div class="campos">
							<?php print drupal_render($formnew['user']['group']);?>
						</div>
					</div>
					<div class="errors" style="display: none"></div>
				</div>
				<div class="right_col">
					<div class="escuela">
					</div>
					<div class="parent" style="display:none">
						<div class="campos">
							<?php print drupal_render($formnew['user']['country']);?>
						</div>
						<div class="campos">
							<?php print drupal_render($formnew['user']['city']);?>
						</div>
						<?php $i=0; while($i <= 2){?>
							<?php print drupal_render($formnew['parents']['id'.$i]);?>
						<div class="campos">
							<?php print drupal_render($formnew['parents']['fullname'.$i]);?>	
						</div>
						<div class="campos">
							<?php print drupal_render($formnew['parents']['occupation'.$i]);?>	
						</div>
						<div class="campos">
							<?php print drupal_render($formnew['parents']['phone'.$i]);?>	
						</div>
						<div class="campos">
							<?php print drupal_render($formnew['parents']['email'.$i]);?>	
						</div>
						<?php $i++;}?>
					</div>
				</div>
				<div class="">
					<?php print drupal_render($formnew['form_build_id'])?>
					<?php print drupal_render($formnew['form_token'])?>
					<?php print drupal_render($formnew['form_id'])?>
				</div>
				<div class="buton_content">
					<?php print drupal_render($formnew['submit'])?>
				</div>
			</div>
			</form>
		</div>
		
		
		<div class="massive fram" style="display:none">
			<div class="down_layout">
				<?php global $base_url?>
				<a href="<?php echo $base_url?>/sites/all/modules/user_sso/CargaMasivaUsuarios.xls" target="_blank">Descargar Plantilla</a>
			</div>
			
			<form enctype="multipart/form-data" action="<?php echo $formass['#action'];?>" method="<?php echo $formass['#method'];?>" id="<?php echo $formass['#id'];?>" >
				<div class="cont_form">	
					<div class="campos">
						<?php print drupal_render($formass['csv_delimiter'])?>
					</div>
					<div class="campos">
						<?php print drupal_render($formass['user_upload'])?>
					</div>
					<div class="alerts" style="display:none">
						<div class="color red"><span>Erroneo</span></div>
						<div class="color yellow"><span>Ignorado</span></div>
						<div class="color green"><span>Correcto</span></div>
						<div class="color brown"><span>Mas de un error</span></div>
					</div>
					<div class="preload" style="display:none"></div>
					<div class="errors" style="display: none"></div>
				</div>
				<div class="">
					<?php print drupal_render($formass['form_build_id'])?>
					<?php print drupal_render($formass['form_token'])?>
					<?php print drupal_render($formass['form_id'])?>
				</div>
				<div class="buton_content">
					<?php print drupal_render($formass['button'])?>
				</div>
				<div class="mensaje_csv"></div>
			</form>
			<script type="application/javascript">
			<?php global $base_url?>
				
				$(function(){
					$('.form-file').bind('change',function(e){
						e.preventDefault();
						$('.layer').fadeIn(400,function(){
							$('.errors').empty();
							var data = new FormData();
							$.each($('#UploadButton')[0].files, function(i, file) {
							    data.append('file-'+i, file);
							});
							
							var error =0;
							var url = '<?php echo $base_url?>/user-sso/preload-list/'+$('#edit-csv-delimiter').val();
							$.ajax({
								url: url,
							    data: data,
							    cache: false,
							    contentType: false,
							    processData: false,
							    type: 'POST',
							    success: function(response){
							    	$('.mensaje').show();
							    	var file = 0;
									$('.preload').html(response);
									$('.preload').fadeIn(400);
									$('.l_tab_prev tr').each(function(){
										$(this).addClass('line_'+file);
										var col = 0;
										$(this).children($('td')).each(function(){
											$(this).addClass('col_'+col);
											col++;
										});
										file++;
									});
									
									$('.col_3').each(function(){
										if($(this).text() != 'Sexo'){
											var sexo = $(this).html().toLowerCase();
											if(sexo == 'h' || sexo == 'm'){
												$(this).addClass('correct');	
											}else{
												$(this).addClass('incorrect');
												error++;
											}
										}
									});
									
									var arrRep = new Array();
									$('.col_4').each(function(){
										if($(this).text() != 'correoElectronico'){
											var mail = validarEmail($(this).text());
											var campo = $(this);
											$.ajax({
												url:'user-sso/validarmail',
												type:'POST',
												data:{mail:''+campo.text()},
												dataType:'json',
												success:function(resultVal){
													if(resultVal.result == 300){
														campo.addClass('incorrect');
														$('.errors').append('<div class="err"> El email '+campo.text()+' es '+resultVal.msg+' </div>');
														error++;
													}else{
														if(mail){
															var comprueba = 0;
															$('.col_4').each(function(){
																
																var campo4 = $(this).text();
																if(campo.text() == campo4){
																	comprueba++
																}
															});
															
															if(comprueba > 1){									
																campo.addClass('incorrect');
																$('.errors').append('<div class="err"> El email '+campo.text()+' se encuentra duplicado</div>');
																error++;
															}else{
																var valor = { name : campo.text() };
																$.ajax({
																	url:'user-sso/existrow',
																	type:'POST',
																	dataType:'json',
																	data:{
																		campos : valor,
																	},
																	success:function(result){
																		if(result == 0){
																			campo.addClass('correct');
																		}else{
																			campo.addClass('incorrect');
																			$('.errors').append('<div class="err"> El email '+campo.text()+' ya se encuentra registrado</div>');
																			error++;
																		}
																	}
																});
															}
														}	
													}
												}
											});
										}
										
									});
									
									$('.col_5').each(function(){
										var campo = $(this);
										if($(this).text() != 'Matricula'){
											var comprueba = 0;
											$('.col_5').each(function(){
												var campo5 = $(this).text();
												if(campo.text() == campo5){
													comprueba++
												}
											});
											console.log(comprueba);
											if(comprueba > 1){
												campo.addClass('incorrect');
												$('.errors').append('<div class="err"> La matricula '+campo.text()+' se duplica</div>');
												error++;
											}else{
												var valor = { matricula : $(this).text() };
												$.ajax({
													url:'user-sso/existrow',
													type:'POST',
													dataType:'json',
													data:{
														campos : valor,
													},
													success:function(result){
														if(result == 0){
															campo.addClass('correct');
														}else{
															campo.addClass('incorrect');
															$('.errors').append('<div class="err"> La matricula '+campo.text()+' ya esta en uso</div>');
															error++;
														}
													}
												});
											}
										}
									});
									
									$('.col_7').each(function(){
										if($(this).text() != 'TipoUsuario'){
											var profesor = $(this).html().toLowerCase();
											if(profesor == 'maestro' || profesor == 'alumno' || profesor == 'profesor' || profesor == 'estudiante'){
												if(profesor == 'maestro' || profesor == 'profesor'){
													var col = 8;
													while(col < 17){
														$(this).parent().find($('.col_'+col)).css({'background-color':'rgba(255, 255, 0, 0.4)'});
														$(this).parent().find($('.col_5')).css({'background-color':'rgba(255, 255, 0, 0.4)'});
														$(this).parent().find($('.col_6')).css({'background-color':'rgba(255, 255, 0, 0.4)'});
														col++;
													}
												}else{
													
												}
												$(this).addClass('correct');
											}else{
												$(this).addClass('incorrect');
												$('.errors').append('<div class="err"> El tipo de usuario '+$(this).text()+' tiene un formato incorrecto</div>');
												error++;
											}
										}
									});
									var each = 0;
									$('.l_tab_prev tr').each(function(){
										var alumno = $(this).find($('td.col_0')).text()+' '+$(this).find($('td.col_1')).text()+' '+$(this).find($('td.col_2')).text();
										var p1 = $(this).find($('td.col_11')).text();
										var p2 = $(this).find($('td.col_15')).text();
				
										if(p1 != '' || p2 != ''){
											
										}else{
											$(this).addClass('incorrect');
											
											$('.errors').append('<div class="err"> Colocar al menos un email de tutor <b>Alumno </b>'+ alumno +'</div>');
											error++;
										}
									})
									
									if(error > 0){
										$('.alerts').slideDown(300)
										$('.errors').fadeIn(300);
									}
									$('.layer').fadeOut(400);
							    }
							});
						});
					});
					
					function validarEmail( email ) {
					    expr = /^([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$/;
					    if(!expr.test(email)){
					        return false;
					    }else{
					    	return true;
					    }
					}
				});
				</script>
		</div>
	</div>
</div>
<style>
.tool_list {
    margin: 10px 0 20px;
}

.left_col {
    float: left;
    width: 60%;
}

.right_col {
    float: left;
    width: 40%;
}

input[type="text"] {
    float: left;
    width: 90%;
}

.send_ceVal {
    float: left;
}

.campos.role {
    float: left;
    margin: 0 0 10px;
    width: 100%;
}

.rols {
    float: left;
    width: 50%;
}


/*preload csv*/
.preload {
    border-radius: 10px;
    box-shadow: 0 0 7px -3px #000;
    float: left;
    height: 300px;
    margin: 20px 0;
    overflow: scroll;
    width: 120%;
}

.tbl_1 .red{
	background:red;
}

.down_layout {
	width:100%;
	float:left;
	margin:0 0 20px 0;
}
.down_layout > a {
    background-color: #ccc;
    border-radius: 5px;
    color: #fff;
    cursor: pointer;
    padding: 5px 10px;
}

.down_layout > a:hover{
	text-decoration:none;
}

.correct {
    background-color: rgba(0, 255, 0, 0.3);
}

.incorrect {
    background-color: rgba(255, 0, 0, 0.3);
}

.alerts .color {
    border-radius: 10px;
    box-shadow: 0 0 3px 0 #000;
    float: left;
    height: 25px;
    margin: 0 5px;
    width: 100px;
}

.alerts .red{
	background-color:rgba(255,0,0,0.4);
}

.alerts .yellow{
	background-color:rgba(255,255,0,0.4);
}

.alerts .green{
	background-color:rgba(0,255,0,0.4);
}

.alerts .brown{
	background-color:rgba(186,133,76,0.4);
}

.color {
    float: left;
    line-height: 25px;
    padding: 0 13px;
    text-align:center;
}

.layer{
	background:rgba(0,0,0,.3);
	position:fixed;
	left:0;
	top:0;
	width:100%;
	height:100%;
}

.mensaje {
    background-image: url("http://cdn.klimg.com/kapanlagi.com/v5/i/ajax-loading-image.gif");
    background-position: center center;
    border-radius: 5px;
    box-shadow: 0 0 7px 6px #fff;
    float: left;
    height: 20px;
    left: 50%;
    margin-left: -100px;
    margin-top: -20px;
    position: absolute;
    top: 50%;
    width: 200px;
}

.buton_content {
    float: left;
    margin: 20px 0 0;
    width: 100%;
}

.errors {
    float: left;
    height: 230px;
    overflow-x: hidden;
    width: 120%;
}

.massive.fram{
	overflow:visible !important; 
}
</style>


