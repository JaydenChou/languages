package MyCheckout;

use 5.006001;
use strict;
use warnings;

require Exporter;

our @ISA = qw(Exporter);

# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.

# This allows declaration	use MyCheckout ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.
our %EXPORT_TAGS = ( 'all' => [ qw(checkoutProduct
	
) ] );

our @EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );

our @EXPORT = qw(checkoutProduct
	
);

our $VERSION = '0.01';

use Google::Checkout::General::GCO;
use Google::Checkout::General::MerchantItem;
use Google::Checkout::General::DigitalContent;
use Google::Checkout::General::ShoppingCart;
use Google::Checkout::XML::CheckoutXmlWriter;
use Google::Checkout::General::MerchantCheckoutFlow;
use Google::Checkout::General::ShippingRestrictions;
use Google::Checkout::General::Pickup;
use Google::Checkout::General::FlatRateShipping;
use Google::Checkout::General::MerchantCalculatedShipping;
use Google::Checkout::General::TaxRule;
use Google::Checkout::General::TaxTable;
use Google::Checkout::General::TaxTableAreas;
use Google::Checkout::General::MerchantCalculations;

use Google::Checkout::XML::Constants;
use Google::Checkout::General::Util qw/is_gco_error/;


# checkout Product
sub checkoutProduct{
        my @items=@_;

#--
#-- Create a CBG object so we can interact with the CBG system passing merchant id, key and server.
#--
	my $gco = Google::Checkout::General::GCO->new(
			merchant_id => '919319089717699', 
			merchant_key => 'zQMpr6uPPleRGxVcx3zXbw', 
			gco_server => 'https://sandbox.google.com/checkout/api/checkout/v2/merchantCheckout/Merchant/919319089717699'
			);


#--
#-- Create a shopping cart that will expire in 1 month
#--
	my $cart = Google::Checkout::General::ShoppingCart->new(
		   expiration    => "+1 month",
		   private       => "Simple shopping cart");


#--
#-- Digital content
#--
	my $digital = Google::Checkout::General::DigitalContent->new(
		      name            => "Digital",
		      description     => "Requires key to download",
		      price           => 19.99,
		      quantity        => 1,
		      delivery_method => Google::Checkout::General::DigitalContent::KEY_URL_DELIVERY,
		      key             => 1234,
		      url             => 'http://download/url');

	$cart->add_item($digital);

#--
#-- Now add the items to the cart
#--
	foreach (@items) {
		$cart->add_item($_);
	}
	


#--
#-- Now checkout...
#--
	my $response = $gco->checkout($cart);

#--
#-- Check for error
#--
	die $response if is_gco_error($response);

#--
#-- No error, the redirect URL is returned to us
#--
	return $response;
}


# Preloaded methods go here.

1;
__END__
# Below is stub documentation for your module. You'd better edit it!

=head1 NAME

MyCheckout - Perl extension for Google Checkout integration.

=head1 SYNOPSIS

	use MyCheckout;
	use Google::Checkout::General::MerchantItem;

	use strict;
	use warnings;

#--
#-- User wants to buy 3 horses and 10 ducks
#-- 


#-- Now we create a merchant item for the horses
	my $cowItem = Google::Checkout::General::MerchantItem->new(
		   name     => "Horse",
		   description => "A Horse" ,
		   price       => 432.0,
		   quantity    => 3,
		   private     => "gold");

#-- Now we create a merchant item for the ducks
	my $chickenItem = Google::Checkout::General::MerchantItem->new(
		   name     => "Duck",
		   description => "A Duck" ,
		   price       => 32.0,
		   quantity    => 10,
		   private     => "gold");

	my @items = ($cowItem, $chickenItem);

	checkoutProduct(@items);

=head1 DESCRIPTION

Here is implemented a Checkout Shopping Cart using Google Checkout Objects that will make a checkout in the Google Checkout Server in Sandbox mode.

As the project is in Perl we are simplifying a little bit the process of integration with Google Checkout, because there are CPAN modules available:
http://search.cpan.org/~dzhuo/Google-Checkout-1.1.1/

The subroutine name is checkoutProduct.

=head2 EXPORT

None by default.



=head1 SEE ALSO

Mention other useful documentation such as the documentation of
related modules or operating system documentation (such as man pages
in UNIX), or any relevant external documentation such as RFCs or
standards.

If you have a mailing list set up for your module, mention it here.

If you have a web site set up for your module, mention it here.

=head1 AUTHOR

Javier Fuchs, E<lt>javier.fuchs@gmail.com<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2011 by Javier Fuchs

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.8.9 or,
at your option, any later version of Perl 5 you may have available.


=cut
