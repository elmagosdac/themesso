<?php //print drupal_render($form);
global $base_url
?>
<script src="<?php echo $base_url?>/sites/all/modules/user_sso/js/jquery-1.11.1.min.js"></script>
<script src="<?php echo $base_url?>/sites/all/modules/user_sso/js/jquery-migrate-1.2.1.min.js"></script>

<form enctype="multipart/form-data" action="<?php echo $form['#action'];?>" method="<?php echo $form['#method'];?>" id="<?php echo $form['#id'];?>" >
	<div class="left_col">	
		<div class="campos">
			<?php print drupal_render($form['isbn'])?>
		</div>
		<div class="campos">
			<?php print drupal_render($form['application'])?>
			
			<div class="c_f">
				<?php print drupal_render($form['dia'])?>
			</div>
			<div class="c_f">
				<?php print drupal_render($form['mes'])?>
			</div>
			<div class="c_f">
				<?php print drupal_render($form['anio'])?>
			</div>
		</div>
		<div class="mensaje" style="display:none"><img src="http://cdn.klimg.com/kapanlagi.com/v5/i/ajax-loading-image.gif"></div>
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
	
	.campos .form-type-select {
	    float: left;
	    margin-right: 20px;
	}
	
	.campos .c_f{
		float:left;
		margin-right:10px;
	}
</style>