<?php //print drupal_render($form);
global $base_url
?>
<script src="<?php echo $base_url?>/sites/all/modules/user_sso/js/jquery-1.11.1.min.js"></script>
<script src="<?php echo $base_url?>/sites/all/modules/user_sso/js/jquery-migrate-1.2.1.min.js"></script>

<div class="down_layout">
	<?php global $base_url?>
	<a href="<?php echo $base_url?>/sites/all/modules/user_sso/CargaMasivaUsuarios.xls" target="_blank">Descargar Plantilla</a>
</div>

<form enctype="multipart/form-data" action="<?php echo $form['#action'];?>" method="<?php echo $form['#method'];?>" id="<?php echo $form['#id'];?>" >
	<div class="left_col">	
		<div class="campos">
			<?php print drupal_render($form['csv_delimiter'])?>
		</div>
		<div class="campos">
			<?php print drupal_render($form['user_upload'])?>
		</div>
		
		<div class="layer" style=" display:none"><div class="mensaje"></div></div>
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
		<?php print drupal_render($form['form_build_id'])?>
		<?php print drupal_render($form['form_token'])?>
		<?php print drupal_render($form['form_id'])?>
	</div>
	<div class="buton_content">
		<?php print drupal_render($form['submit'])?>
	</div>
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
				var url = '<?php echo $base_url?>/user-sso/preload-list/'+$('#edit-csv-delimiter').val();
				var error =0;
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
									url:'validarmail',
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
														url:'existrow',
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
										url:'existrow',
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
	<style>
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
	</style>