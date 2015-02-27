<?php //print drupal_render($form);
global $base_url
?>
<script src="<?php echo $base_url?>/sites/all/modules/user_sso/js/jquery-1.11.1.min.js"></script>
<script src="<?php echo $base_url?>/sites/all/modules/user_sso/js/jquery-migrate-1.2.1.min.js"></script>
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
			$.ajax({
				url:'validarmail',
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
		
		$('input[value=4]').bind('change',function(){
			studenCheck();
			
			if($(this).attr('checked') == 'checked'){
				$('.role input[value=3],.role input[value=7],.role input[value=6],.role input[value=5]').attr('disabled','disabled');
				$('.role input[value=3],.role input[value=7],.role input[value=6],.role input[value=5]').removeAttr('checked');
			}else{
				$('.role input[value=3],.role input[value=7],.role input[value=6],.role input[value=5]').removeAttr('disabled');
			}
		});
		
		$('input[value=6]').bind('change',function(){
			if($(this).attr('checked') == 'checked'){
				$('.user_data').slideUp(300,function(){
					$('.input_ce').fadeIn(300);
				});
				$('.buton_content').fadeOut(300);
				$('.role input[value=3],.role input[value=7],.role input[value=4],.role input[value=5]').attr('disabled','disabled');
			}else{
				$('.user_data').fadeIn(300);
				$('.buton_content').fadeIn(300);
				$('.role input[value=3],.role input[value=7],.role input[value=4],.role input[value=5]').removeAttr('disabled');
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
		
		$('#<?php echo $form['#id']?>').submit(function(e){
			e.preventDefault();
			$('.messages.status').remove();
			var req = true;
			$('.required').each(function(){
				if($(this).val() == ''){
					$(this).css({'box-shadow':'0 0 4px -2px red'});
					req = false;
				}else{
					$(this).css({'box-shadow':''});
				}
			});			
			if(req){
				$.ajax({
					type:'POST',
					url:'<?php echo $form['#action'];?>',
					data: $(this).serialize(),
					success:function(dataBack){
						console.log(dataBack);
						var result;
						if(dataBack == false ){
							alert('El Email ya esta registrado');
							$('input[name=mail]').css({'box-shadow':'0 0 4px -2px red'}).focus();
						}else{
							alert('Creacion exitosa');
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
<div class="cont_token_form">
	<form action="<?php echo $form['#action'];?>" method="<?php echo $form['#method'];?>" id="<?php echo $form['#id'];?>">
	<div class="left_col">
		<div class="campos">
			<?php print drupal_render($form['user']['uid'])?>
		</div>
		<div class="campos role">
			<?php $listRol = $form['user']['role']['#value'];?>
			<?php foreach($listRol as $kr=>$lr){?>
				<input name="role[]" type="checkbox" value="<?php echo $kr?>">
				<span><?php echo $lr?></span>
			<?php }?>
		</div>
		<div class="input_ce" style="display:none">
			Validacion de Licencia
			<input type="text" class="ce_val" placeholder="Licencia">
			<button class="send_ceVal">Validar</button>
			
		</div>
		<div class="imgload" style="display:none"></div>
		<script>
			$(function(){
				$('.send_ceVal').click(function(e){
					e.preventDefault();
					$('.imgload').fadeIn(200);
					$.ajax({
						url:'soapajax',
						type:'POST',
						data:{
							contactid: ''+$('.ce_val').val(),
							username: 'Denumeris',
							password: 'D3num3r1s',
							method:'GetContact'
						},
						dataType:'json',
						success:function(xmlResult){
							var mobj = $.parseJSON(xmlResult.GetContactResult);
							var acccount = '';
							$.each(mobj,function(k,obj){
								console.log(obj);
								$('#edit-mail').attr('value',obj.email);
								$('#edit-username').attr('value',obj.firstname);
								$('#edit-lastname').attr('value',obj.lastname);
								account = obj.accountid;
								$('.license').attr('value',account);
							});
							
							$.ajax({
								url:'soapajax',
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
				<?php print drupal_render($form['user']['username'])?>
			</div>
			<div class="campos">
				<?php print drupal_render($form['user']['middlename'])?>
			</div>
			<div class="campos">
				<?php print drupal_render($form['user']['lastname'])?>
			</div>
			<div class="campos">
				<?php print drupal_render($form['user']['matricula'])?>
			</div>
			<div class="campos">
				<?php print drupal_render($form['user']['mail'])?>
			</div>
			<div class="campos apps" style="display:none">
				<?php $listRol = $form['user']['apps']['#value'];?>
				<?php foreach($listRol as $kr=>$lr){?>
					<input style="display: none" name="apps[]" type="checkbox" value="<?php echo $kr?>">
					<span style="display: none"  value="<?php echo $kr?>"><?php echo $lr?></span>
				<?php }?>
			</div>
		</div>
		<div class="errors" style="display: none"></div>
	</div>
	<div class="right_col">
		<div class="escuela">
			
		</div>
		<div class="parent" style="display:none">
			<?php $i=0; while($i <= 2){?>
				<?php print drupal_render($form['parents']['id'.$i]);?>
			<div class="campos">
				<?php print drupal_render($form['parents']['fullname'.$i]);?>	
			</div>
			<div class="campos">
				<?php print drupal_render($form['parents']['occupation'.$i]);?>	
			</div>
			<div class="campos">
				<?php print drupal_render($form['parents']['phone'.$i]);?>	
			</div>
			<?php $i++;}?>
		</div>
	</div>
	<div class="">
		<?php print drupal_render($form['form_build_id'])?>
		<?php print drupal_render($form['form_token'])?>
		<?php print drupal_render($form['form_id'])?>
	</div>
	<div class="buton_content">
		<?php print drupal_render($form['submit'])?>
	</div>
</div>
<style>
.imgload{
	background-image: url("http://cdn.klimg.com/kapanlagi.com/v5/i/ajax-loading-image.gif");
	background-repeat:no-repeat;
	background-position:center center;
	height:30px;
	width:200px;
}
.cont_token_form {
    float: left;
    width: 100%;
}

.left_col {
    float: left;
    width: 60%;
}

.right_col {
	position:relative;
    float: left;
    width: 40%;
}
.buton_content {
    float: left;
    text-align: right;
    width: 100%;
}
#resutado_e_cat {
    margin: 0 auto;
    text-align: left;
    width: 80%;
}

#resutado_e_cat img {
    border-radius: 10px;
    box-shadow: 0 0 5px -2px #1b1b1b;
    width: 80%;
}

.cont_token_form input[type=text] {
	width: 90%;
}


.buton_content .form-submit {
    border-radius: 10px;
    float: right;
    font-size: 1em;
    margin-right: 160px;
}

</style>