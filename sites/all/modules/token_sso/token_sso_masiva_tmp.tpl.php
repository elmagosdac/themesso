<?php //print drupal_render($form);
global $base_url
?>
<script src="<?php echo $base_url?>/sites/all/modules/user_sso/js/jquery-1.11.1.min.js"></script>
<script src="<?php echo $base_url?>/sites/all/modules/user_sso/js/jquery-migrate-1.2.1.min.js"></script>

<form enctype="multipart/form-data" action="<?php echo $form['#action'];?>" method="<?php echo $form['#method'];?>" id="<?php echo $form['#id'];?>" >
	<div class="down_layout">
		<?php global $base_url?>
		<a href="<?php echo $base_url?>/sites/all/modules/token_sso/LauOutToken.csv" target="_blank">Descargar Plantilla</a>
	</div>
	<div class="left_col">	
		<div class="campos">
			<?php print drupal_render($form['token_upload'])?>
		</div>
		<div class="campos">
			<?php print drupal_render($form['csv_delimiter'])?>
		</div>
		<div id="resultado_aviso"></div>
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
<style>
	.campos {
	    float: left;
	    width: 100%;
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
</style>