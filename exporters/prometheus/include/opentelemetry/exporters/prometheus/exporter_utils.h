// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <prometheus/metric_family.h>
#include <string>
#include <vector>
#include "opentelemetry/metrics/provider.h"
#include "opentelemetry/sdk/metrics/meter.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace exporter
{
namespace metrics
{
/**
 * The Prometheus Utils contains utility functions for Prometheus Exporter
 */
class PrometheusExporterUtils
{
public:
  /**
   * Helper function to convert OpenTelemetry metrics data collection
   * to Prometheus metrics data collection
   *
   * @param records a collection of metrics in OpenTelemetry
   * @param populate_target_info whether to populate target_info
   * @return a collection of translated metrics that is acceptable by Prometheus
   */
  static std::vector<::prometheus::MetricFamily> TranslateToPrometheus(
      const sdk::metrics::ResourceMetrics &data,
      bool populate_target_info = true);

private:
  /**
   * Append key-value pair to prometheus labels.
   *
   * @param name label name
   * @param value label value
   * @param labels target labels
   */
  static void AddPrometheusLabel(std::string name,
                                 std::string value,
                                 std::vector<::prometheus::ClientMetric::Label> *labels);

  static opentelemetry::sdk::metrics::AggregationType getAggregationType(
      const opentelemetry::sdk::metrics::PointType &point_type);

  /**
   * Translate the OTel metric type to Prometheus metric type
   */
  static ::prometheus::MetricType TranslateType(opentelemetry::sdk::metrics::AggregationType kind,
                                                bool is_monotonic = true);

  /**
   * Add a target_info metric to collect resource attributes
   */
  static void SetTarget(const sdk::metrics::ResourceMetrics &data,
                        const opentelemetry::sdk::instrumentationscope::InstrumentationScope *scope,
                        std::vector<::prometheus::MetricFamily> *output);

  /**
   * Set metric data for:
   * Counter => Prometheus Counter
   */
  template <typename T>
  static void SetData(std::vector<T> values,
                      const opentelemetry::sdk::metrics::PointAttributes &labels,
                      const opentelemetry::sdk::instrumentationscope::InstrumentationScope *scope,
                      ::prometheus::MetricType type,
                      ::prometheus::MetricFamily *metric_family,
                      const opentelemetry::sdk::resource::Resource *resource);

  /**
   * Set metric data for:
   * Histogram => Prometheus Histogram
   */
  template <typename T>
  static void SetData(std::vector<T> values,
                      const std::vector<double> &boundaries,
                      const std::vector<uint64_t> &counts,
                      const opentelemetry::sdk::metrics::PointAttributes &labels,
                      const opentelemetry::sdk::instrumentationscope::InstrumentationScope *scope,
                      ::prometheus::MetricFamily *metric_family,
                      const opentelemetry::sdk::resource::Resource *resource);

  /**
   * Set time and labels to metric data
   */
  static void SetMetricBasic(
      ::prometheus::ClientMetric &metric,
      const opentelemetry::sdk::metrics::PointAttributes &labels,
      const opentelemetry::sdk::instrumentationscope::InstrumentationScope *scope,
      const opentelemetry::sdk::resource::Resource *resource);

  /**
   * Convert attribute value to string
   */
  static std::string AttributeValueToString(
      const opentelemetry::sdk::common::OwnedAttributeValue &value);

  /**
   * Handle Counter and Gauge.
   */
  template <typename T>
  static void SetValue(std::vector<T> values,
                       ::prometheus::MetricType type,
                       ::prometheus::ClientMetric *metric);

  /**
   * Handle Gauge from MinMaxSumCount
   */
  static void SetValue(double value, ::prometheus::ClientMetric *metric);

  /**
   * Handle Histogram
   */
  template <typename T>
  static void SetValue(std::vector<T> values,
                       const std::vector<double> &boundaries,
                       const std::vector<uint64_t> &counts,
                       ::prometheus::ClientMetric *metric);
};
}  // namespace metrics
}  // namespace exporter
OPENTELEMETRY_END_NAMESPACE
