<?php //print drupal_render($form);
function nivelesNumber($level){
	$id = 0;
	$level = str_replace(' ', '', strtolower($level)); 
	switch ($level) {
		case 'preescolar':
			$id = 1;
			break;
		case 'primaria':
			$id = 2;
			break;
		case 'secundaria':
			$id = 3;
			break;
		case 'bachillerato':
			$id = 4;
			break;
		case 'preparatoria':
			$id = 4;
			break;
	}
	
	return @$id;
}
global $base_url;
?>	

<script src="<?php echo $base_url?>/sites/all/modules/user_sso/js/jquery-1.11.1.min.js"></script>
<script src="<?php echo $base_url?>/sites/all/modules/user_sso/js/jquery-migrate-1.2.1.min.js"></script>
<script type="application/javascript">
	$(function(){
		$('.level').on('change',function(){
			var html = '';
			var cicle = 0;
			
			switch($(this).val()){
				case '1':
					cicle = 3;
					break;
				case '2':
					cicle = 6;
					break;
				case '3':
					cicle = 3;
					break;
				case '4':
					cicle = 3;
					break;
					
			}
			var count = 1;
			while(count <= cicle){
				html +='<option>'+count+'</option>';
				count++;
			}
			
			$('.grade').html(html);
		});
		
		$('#<?php echo $form['#id'];?>').on('submit',function(e){
			e.preventDefault();
			$.ajax({
				url:'<?php echo $form['#action'];?>',
				type:'POST',
				data:$(this).serialize(),
				dataType:'json',
				success:function(resp){
					if(resp.code == 200){
						alert(resp.mensaje);
						document.getElementById("<?php echo $form['#id'];?>").reset();
						$('.grade').html('<option></option>');
					}else{
						alert(resp.mensaje);
					}
				}
			})
		});
	});
</script>
<div class="cont_token_form">
	<form action="<?php echo $form['#action'];?>" method="<?php echo $form['#method'];?>" id="<?php echo $form['#id'];?>">
	<div class="left_col">
		<div class="campos">
			<?php print drupal_render($form['group']['name'])?>
		</div>
		<div class="campos">
			<label>Nivel</label>
			<select name="level" class="level">
				<option></option>
				<?php foreach($niveles as $n):?>
					<option value="<?php echo nivelesNumber($n)?>"><?php echo $n?></option>	
				<?php endforeach;?>
			</select>
			<div class="description">Selecciona el nivel de estudios</div>
		</div>
		<div class="campos">
			<label>Grado</label>
			<select name="grade" class="grade">
				<option></option>
			</select>
			<div class="description">Selecciona el grado de estudios</div>
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
.campos{
	margin:5px 0;
}
</style>